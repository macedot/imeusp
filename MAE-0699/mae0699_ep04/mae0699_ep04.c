/******************************************************************************
 * Nome  : Thiago Pinheiro de Macedo
 * N USP : 5124272
 ******************************************************************************
 * MAE0699 - Tópicos de Probabilidade e Estatística
 * Prof.: José Carlos Simon de Miranda
 * Exercicio de Implementação #04 (Simulacao de Atendimento a uma Fila)
 *****************************************************************************/

/******************************************************************************
 * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ATENCAO <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< *
 *    Este programa tem como objetivo gerar arquivos em disco, contendo 
 *    dados para analise estatistica das simulacoes e estes arquivos podem 
 *    ser grandes. Cuidado com a sua cota!! =]
 * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ATENCAO <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< *
 *****************************************************************************/

/******************************************************************************
 * Definicoes internas
 *****************************************************************************/
#define USE_SFMT          (1) /* Usa SFMT para geracao de numeros aleatorios */

#define TIPO_ELEM_PREF    (0) /* Elemento com preferencia de atendimento; */
#define TIPO_ELEM_COMUM   (1) /* Elemento comuns; */
#define TIPO_ELEM_MAX     (2) /* Total de tipos de elementos possivels; */

#define TIPO_PROC_PREF    (0) /* Processador preferencial;  */
#define TIPO_PROC_COMUM   (1) /* Processador comum;  */
#define TIPO_PROC_MAX     (2) /* Total de tipos de processadores possivels; */

/******************************************************************************
 * Includes da aplicacao;
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

#if USE_SFMT
#  include "SFMT.h"
#else

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#   include <inttypes.h>
#elif defined(_MSC_VER)
#  ifndef uint32_t
     typedef unsigned int uint32_t;
#  endif
#  ifndef uint64_t
     typedef unsigned long long uint64_t;
#  endif
#else
#   include <inttypes.h>
#endif

#endif /* USE_SFMT */

#ifdef WIN32
#  include <windows.h>
#else

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef BOOL
typedef int                 BOOL;
#endif

#endif

#ifndef M_SQRT2
#define M_SQRT2    1.41421356237309504880168872421      /* sqrt(2) */
#endif

/******************************************************************************
 * Tipos internos da aplicacao;
 *****************************************************************************/
typedef struct elemento {
  int       nTipo;
  uint32_t  nTempoEspera;
  uint32_t  nTempoAtende;
} Elemento; 

typedef struct elemLista {
  Elemento*          pElem;
  struct elemLista*  pNext;
} ElemLista; 

typedef struct filaElem {
  ElemLista*  pLista;
  ElemLista*  pLast;
  uint32_t    nQuant[TIPO_ELEM_MAX];
  uint32_t    nTam;
} FilaElem; 

typedef struct procElem {
  uint32_t   nTotOcioso;
  uint32_t   nTempoAtende;
  ElemLista* pElemLista;
} ProcElem; 

/******************************************************************************
 * Constantes da aplicacao
 *****************************************************************************/
#define TEMPO_SIMULACAO    (480)

/******************************************************************************
 * Variaveis globais Internas
 *****************************************************************************/
uint32_t  nClock;             /* Clock da simulacao; */
uint32_t  nRepeatSim   = 1;
uint32_t  nNumProc     = 4;
uint32_t  nNumProcPref = 1;
FilaElem  sFilaPend;
FilaElem  sFilaAtend;
ProcElem* pProcElem;

/* Tabela de valores de Lambda (casos 1 e 2); */
const double fTxCheg[2][TIPO_ELEM_MAX] = {
  { 1.0 / 6.0 , 1.0 },
  { 1.0 / 12.0, 0.5 },
};

/* Tabela de valores de Lambda (casos 3 e 4); */
/* const double fTxFuncLambda[TEMPO_SIMULACAO] = { }; */

/**
 * Gera a taxa lambda, dada por uma funcao (Utilizada pelos Casos 3 e 4);
 */
/*
static double funcTaxaLambda(uint32_t  t)
{
  static double fLambda;
  static double fAux;
  static const double C1 = (3.0 / 3600.0);

  fAux = (t - 120.0) * (t - 240.0);
  fLambda = (fAux > 0  ? (C1 * fAux) : 0.0);
  fLambda += 1.0 + (t - 240.0) * (t - 240.0) / 57600.0;
  fLambda /= 880.0;
  return fLambda;
}
*/
/******************************************************************************
 * 
 *****************************************************************************/
static double getTaxaSim(uint32_t nCaso, int nTipo)
{
  double fLambda = 0.0;

  if((nCaso >= 0) && (nCaso <= 1))
  {
    return fTxCheg[nCaso][nTipo];
  }
  /* fLambda = funcTaxaLambda(nClock); */
/*
  fLambda = fTxFuncLambda[nClock];
  if((nCaso == 2) && (nTipo == TIPO_ELEM_PREF))
  {
    fLambda /= 6.0;
  }
  else if(nCaso == 3)
  {
    fLambda /= (nTipo == TIPO_ELEM_PREF) ? 12.0 : 2.0;
  }
*/
  return fLambda;
}

/******************************************************************************
 * 
 *****************************************************************************/
uint32_t getTime(void)
{
#if defined(__GNUC__)
  return (uint32_t)(1000 * (clock() /  (double)CLOCKS_PER_SEC));
#else
  return (uint32_t)GetTickCount();
#endif
}

/******************************************************************************
 * Funcoes para geracao de Variaveis Aleatorias
 *****************************************************************************/
void randInic(void)
{
#if USE_SFMT
  init_gen_rand(getTime());
#else
  srand(getTime());
#endif
}

double randUniform(void)
{
#if USE_SFMT
  return (double)genrand_res53();
#else
  return (double)(rand() / (double)RAND_MAX);
#endif
}

double randUniformPos(void)
{
  double u;

  do {
    u = randUniform();
  } while (u == 0.0);
  return u;
}

/******************************************************************************
 * double randPoisson(const double fLambda)
 * Como implementado na GSL: http://www.gnu.org/software/gsl/
 *
 * "The poisson distribution has the form
 *   p(n) = (fLambda ^ n / n!) exp(-fLambda) 
 *   for n = 0, 1, 2, ... . The method used here is the one from Knuth.
 *****************************************************************************/
uint32_t randPoisson(const double fLambda)
{
  uint32_t  k = 0;
  double    prod = 1.0;
  double    expLambda = exp(-fLambda);

  do {
    prod *= randUniformPos();
    k++;
  } while(prod > expLambda);
  return (k - 1);
}
/****************************************************************************** 
 * The lognormal distribution has the form 
 * 
 * p(x) dx = 1/(x * sqrt(2 pi sigma^2)) exp(-(ln(x) - zeta)^2/2 sigma^2) dx
 * 
 * for x > 0. Lognormal random numbers are the exponentials of
 * gaussian random numbers 
 *****************************************************************************/
double randLogNormal(const double zeta, const double sigma)
{
  double u, v, r2, normal, z;

  do {
    /* choose x,y in uniform square (-1,-1) to (+1,+1) */
    u = -1 + 2 * randUniform();
    v = -1 + 2 * randUniform();
    /* see if it is in the unit circle */
    r2 = u * u + v * v;
  } while (r2 > 1.0 || r2 == 0.0);
  normal = u * sqrt (-2.0 * log (r2) / r2);
  z =  exp (sigma * normal + zeta);
  return z;
}

/******************************************************************************
 * Funcoes da Fila
 *****************************************************************************/
void Fila_AppendElem(FilaElem* pFilaElem, ElemLista* pElemLista)
{
  pElemLista->pNext = NULL;
  if(pFilaElem->pLista == NULL)
  {
    pFilaElem->pLast = pFilaElem->pLista = pElemLista;
  }
  else
  {
    pFilaElem->pLast->pNext = pElemLista;
    pFilaElem->pLast = pFilaElem->pLast->pNext;
  }
  pFilaElem->nQuant[pElemLista->pElem->nTipo]++;
  pFilaElem->nTam++;
}

void Fila_Inclui(FilaElem* pFilaElem, int nTipoElem)
{
  ElemLista*  pElemLista;

  pElemLista = (ElemLista*)malloc(sizeof(ElemLista));
  if(pElemLista != NULL)
  {
    pElemLista->pElem = (Elemento*)malloc(sizeof(Elemento));
    if(pElemLista->pElem != NULL)
    {
      pElemLista->pElem->nTipo        = nTipoElem;
      pElemLista->pElem->nTempoAtende = 0;
      pElemLista->pElem->nTempoEspera = nClock;
      Fila_AppendElem(pFilaElem, pElemLista);
    }
    else
    {
      free(pElemLista);
    }
  }
}

void Fila_LiberaFilaElem(FilaElem* pFilaElem)
{
  ElemLista*  pNext;

  while(pFilaElem->pLista != NULL)
  {
    pNext = pFilaElem->pLista->pNext;
    free(pFilaElem->pLista);
    pFilaElem->pLista = pNext;
  } 
}

ElemLista* Fila_PegaProximo(FilaElem* pFilaElem)
{
  ElemLista*  pElemLista = NULL;

  if(pFilaElem->nTam > 0)
  {
    pElemLista = pFilaElem->pLista;
    /* 
     * - O atual nao eh um tipo preferencial;
     * - Existe algum preferecial na fila;
     */
    if((pElemLista->pElem->nTipo != TIPO_ELEM_PREF) && 
       (pFilaElem->nQuant[TIPO_ELEM_PREF] > 0))
    {
      pElemLista = pElemLista->pNext;
      while(pElemLista != NULL)
      {
        if(pElemLista->pElem->nTipo == TIPO_ELEM_PREF)
        {
          break;
        }
        pElemLista = pElemLista->pNext;
      }
    }
  }
  return pElemLista;
}

ElemLista* Fila_AtendeProximo(void)
{
  ElemLista*  pElemLista = NULL;

  if(sFilaPend.nTam > 0)
  {
    pElemLista = sFilaPend.pLista;
    sFilaPend.pLista = sFilaPend.pLista->pNext;
    sFilaPend.nQuant[pElemLista->pElem->nTipo]--;
    sFilaPend.nTam--;
    if(sFilaPend.nTam == 0)
    {
      sFilaPend.pLast = NULL;
    }
    Fila_AppendElem(&sFilaAtend, pElemLista);
  }
  return pElemLista;
}

/******************************************************************************
 * Funcoes do Processador
 *****************************************************************************/
BOOL Proc_Ocupado(ProcElem* pProcElem)
{
  uint32_t  nTempoAtende;

  if(pProcElem->pElemLista == NULL)
    return FALSE;

  nTempoAtende = (nClock - pProcElem->pElemLista->pElem->nTempoAtende);
  /* Verifica se esgotou o tempo de atendimento; */
  if(nTempoAtende >= pProcElem->nTempoAtende)
  {
    /* Atualiza tempo de atendimento do cliente; */
    pProcElem->pElemLista->pElem->nTempoAtende = nTempoAtende;
    /* Elimina associacao com o Elemento em atendimento; */
    pProcElem->pElemLista = NULL;
  }
  return (pProcElem->pElemLista != NULL);
}

BOOL Proc_Atende(ProcElem* pProcElem, ElemLista* pElemLista)
{
  pProcElem->pElemLista = pElemLista;
  switch(pElemLista->pElem->nTipo)
  {
    case TIPO_ELEM_PREF :
      pProcElem->nTempoAtende = (uint32_t)ceil(randLogNormal(5.0, M_SQRT2));
      if(pProcElem->nTempoAtende > 11) 
      {
        pProcElem->nTempoAtende = 11;
      }
      break;
    case TIPO_ELEM_COMUM :
    default:
      pProcElem->nTempoAtende = (uint32_t)ceil(randLogNormal(3.0, 1.0));
      if(pProcElem->nTempoAtende > 6)  
      {
        pProcElem->nTempoAtende = 6;
      }
      break;
  }

  pProcElem->pElemLista->pElem->nTempoEspera = (nClock - pProcElem->pElemLista->pElem->nTempoEspera);
  pProcElem->pElemLista->pElem->nTempoAtende = nClock;
  return TRUE;
}

void Proc_Atualiza(ProcElem* pProcElem)
{
  /* Verifica se o Processador esta em atendimento; */
  if(!Proc_Ocupado(pProcElem))
  {
    /* Processador Ocioso */
    pProcElem->nTotOcioso++;
  }
}

/******************************************************************************
 * Funcao Principal
 *****************************************************************************/
int main(int argc, char* argv[])
{
  ElemLista*  pElemLista;
  uint32_t    nRepeat;
  uint32_t    nChegada;
  uint32_t    nCasoSimulacao=0xF; /* Todos, por padrao; */
  uint32_t    nCaso;
  uint32_t    nUID;
  uint32_t    i, j;
  BOOL        bFull;
  BOOL        bFim;
  FILE*       fpArqFila;
  FILE*       fpArqElem;
  FILE*       fpArqProc;
  FILE*       fpArqTime;
  char        szArqName[256];
  
  randInic();
  for(i = 0; i < 70; i++) printf("=");
  printf("\nSimulacao de Atendimento a uma Fila\n");
  for(i = 0; i < 70; i++) printf("=");
  /* Trata parametros na linha de comando; */
  for(i = 1; i < (uint32_t)argc; i++)
  {
    if(*argv[i] == '-')
    {
      switch(toupper(*(argv[i]+1)))
      {
        case 'C':
        {
          /** 
           * Casos de simulacao selecionados:
           * +----Bit do Caso 4
           * |+---Bit do Caso 3
           * ||+--Bit do Caso 2
           * |||+-Bit do Caso 1
           * 0000
           * Ex:
           * 1100 = 8|4| |  = 12 = (Caso 3 e 4)
           * 1001 = 8| | |1 =  9 = (Caso 1 e 4)
           * 0010 =  | |2|  =  2 = (Caso 2)
           * 1111 = 8|4|2|1 = 15 = (TODOS)
           */
          nCasoSimulacao = (uint32_t)atol(argv[i]+2);
          break;
        }
        case 'R':
        {
          /* Repeticoes da simulacao (em decimal) */
          nRepeatSim = (uint32_t)atol(argv[i]+2);
          break;
        }
        case 'N':
        {
          /* Numero de processadores (total); */
          nNumProc = (uint32_t)atol(argv[i]+2);
          break;
        }
        case 'P':
        {
          /* Numero de processadores (preferenciais); */
          nNumProcPref = (uint32_t)atol(argv[i]+2);
          break;
        }
      }
    }
  }
  printf("\n\n");
  /* Valida Parametros de simulacao */
  if(nNumProc <= 0)  return 1;
  if(nNumProcPref > nNumProc)  nNumProcPref = nNumProc;
  /* Aloca processadores */
  pProcElem = (ProcElem*)malloc(nNumProc * sizeof(ProcElem));
  if(pProcElem == NULL)
  {
    return 1;
  }
  /* "Unique" ID da simulacao; */
  nUID = getTime();
  /* Trata casos de Simulacao; */
  /* for(nCaso = 0; nCaso < 4; nCaso++) */
  for(nCaso = 0; nCaso < 2; nCaso++)
  {
    /* Verifica se o caso foi selecionado; */
    if((nCasoSimulacao & (1 << nCaso)) == 0) continue; 
    /* Abre arquivos de "despejo" */
    sprintf(szArqName, "fila_%d_%d.txt", (nCaso + 1), nUID);
    fpArqFila = fopen(szArqName,"w+b");
    sprintf(szArqName, "proc_%d_%d.txt", (nCaso + 1), nUID);
    fpArqProc = fopen(szArqName,"w+b");
    sprintf(szArqName, "time_%d_%d.txt", (nCaso + 1), nUID);
    fpArqTime = fopen(szArqName,"a+b");
    for(nRepeat = 0; nRepeat < nRepeatSim; nRepeat++)
    {
      /***************************************************/
      memset(&sFilaPend , 0, sizeof(FilaElem));
      memset(&sFilaAtend, 0, sizeof(FilaElem));
      memset(pProcElem  , 0, nNumProc * sizeof(ProcElem));
      /***************************************************/
      /* Processa a simuacao; */
      bFim = FALSE;
      for(nClock = 0; (bFim == FALSE); nClock++)
      {
        /* Verifica se ainda pode chegar alguem; */
        if(nClock < TEMPO_SIMULACAO) 
        {
          /* Simula Elementos chegando na fila; */
          for(i = 0; i < TIPO_ELEM_MAX; i++)
          {
            nChegada = randPoisson(getTaxaSim(nCaso, i));
            for(j = 0; j < nChegada; j++)
            {
              Fila_Inclui(&sFilaPend, i);
            }
          }
        }
        /***************************************************/
        /* Verifica se a fila nao esta vazia; */
        if(sFilaPend.nQuant > 0)
        {
          /* Simula processamento dos processadores preferenciais; */
          for(i = 0; i < nNumProcPref; i++)
          {
            if(Proc_Ocupado(&pProcElem[i]))  continue;
            /* Obtem - "preferencialmente" - proximo elemento da fila; */
            pElemLista = Fila_PegaProximo(&sFilaPend);
            if(pElemLista == NULL)  break; /* Fila vazia (?!) */
            /* Verifica se o elemento nao eh preferencial; */
            if(pElemLista->pElem->nTipo != TIPO_ELEM_PREF)
            {
              bFull = TRUE;
              /* Verifica disponibilidade dos processadores comuns; */
              for(j = nNumProcPref; j < nNumProc; j++)
              {
                if(!Proc_Ocupado(&pProcElem[j]))
                {
                  /* Processadores comuns ocioso, entao
                     Processador preferencial nao processa este elemento; */
                  bFull = FALSE;
                  break;
                }
              }
              if(!bFull)  break;
            }
            /* Processador atende o proximo da fila; */
            pElemLista = Fila_AtendeProximo();
            Proc_Atende(&pProcElem[i], pElemLista);
          }
          /* Simula processamento dos processadores comuns; */
          for(i = nNumProcPref; i < nNumProc; i++)
          {
            if(Proc_Ocupado(&pProcElem[i]))  continue;
            /* Atende o proximo elemento (se for o caso); */
            pElemLista = Fila_AtendeProximo();
            if(pElemLista == NULL)  break;
            Proc_Atende(&pProcElem[i], pElemLista);
          }
        }
        /***************************************************/
        /* Verifica se ja excedeu o tempo minimo de simulacao; */
        if(nClock >= TEMPO_SIMULACAO) 
        {
          /* Basta aguardar que todos os atendimentos finalizem; */
          bFim = (sFilaPend.nTam == 0);
          for(i = 0; bFim && (i < nNumProc); i++)
          {
            if(Proc_Ocupado(&pProcElem[i]))
            {
              /* Este ainda nao terminou, entao nao vamos finalizar ainda; */
              bFim = FALSE;
            }
          }
        }
        /***************************************************/
        /* Grava tempo ocioso acumulado dos processadores; */
        for(i = 0; i < nNumProc; i++)
        {
          Proc_Atualiza(&pProcElem[i]);
          fprintf(fpArqProc, "%d ", pProcElem[i].nTotOcioso);
        }
        fprintf(fpArqProc, "\n");
        /* Grava arquivo para estatisticas a posteriori; */
        fprintf(fpArqFila, "%d\n", sFilaPend.nTam);
      }
      /***************************************************/
      /* Grava tempo total de simulacao; */
      fprintf(fpArqTime, "%d\n", nClock);
      /***************************************************/
      /* Cria nome de arquivo para dump das informacoes; */
      sprintf(szArqName, "elem_%d_%d.txt", (nCaso + 1), nUID);
      fpArqElem = fopen(szArqName,"a+b");
      pElemLista = sFilaAtend.pLista;
      while(pElemLista != NULL)
      {
        fprintf(fpArqElem, "%d %d\n", pElemLista->pElem->nTipo, pElemLista->pElem->nTempoEspera);
        pElemLista = pElemLista->pNext;
      }
      fclose(fpArqElem);
      /***************************************************/
      /* Libera filas para proxima simulacao; */
      Fila_LiberaFilaElem(&sFilaAtend);
      Fila_LiberaFilaElem(&sFilaPend);
    }
    fclose(fpArqTime);
    fclose(fpArqProc);
    fclose(fpArqFila);
  }
  return 0;
}
