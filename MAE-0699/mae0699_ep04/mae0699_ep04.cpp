//*****************************************************************************
// Nome  : Thiago Pinheiro de Macedo
// N USP : 5124272
//*****************************************************************************
//*****************************************************************************
// MAE0699 - Tópicos de Probabilidade e Estatística
// Prof.: José Carlos Simon de Miranda
// Exercicio de Implementação #04 (Simulacao de Atendimento a uma Fila)
//*****************************************************************************


/******************************************************************************
 * Includes da aplicacao;
 ******************************************************************************/
#include "mae0699_ep04.h"


//*****************************************************************************
// Variaveis globais Internas
//*****************************************************************************
CRand     cRand;
uint32_t  nCasoSimulacao = 0;

//*****************************************************************************
// Funcao Principal
//*****************************************************************************
//-----------------------------------------------------------------------------
// int main(int argc, char* argv[])
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  CProc  cProcessador[NUM_PROC];
  CFila         cFila;
  CElem*    pElemento;
  BOOL      bFull;
  uint32_t  i;
  uint32_t  j;
  uint32_t  nChegada;
  uint32_t  nClock;
  double    fLambda[TIPO_ELEM_MAX];
  
  for(i = 0; i < 70; i++) printf("=");
  printf("\nSimulacao de Atendimento a uma Fila\n");
  for(i = 0; i < 70; i++) printf("=");

  for(i = 1; i < (uint32_t)argc; i++)
  {
    if(*argv[i] == '-')
    {
      switch(toupper(*(argv[i]+1)))
      {
        case 'C':
        {
          nCasoSimulacao = (uint32_t)atol(argv[i]+2);
          break;
        }
      }
    }
  }
  printf("\n\n");

  fLambda[TIPO_ELEM_PREF]  = 1.0 / 6.0;
  fLambda[TIPO_ELEM_COMUM] = 1.0;
  // Processa a simuacao;
  for(nClock = 0; nClock < TEMPO_SIMULACAO; nClock++)
  {
    for(i = 0; i < TIPO_ELEM_MAX; i++)
    {
      nChegada = cRand.randPoisson(fLambda[i]);
      for(j = 0; j < nChegada; j++)
      {
        cFila.Inclui((TIPOELEM)i, nClock);
      }
    }


    if(cFila.Pendentes() > 0)
    {
      for(i = 0; i < NUM_PROC_PREF; i++)
      {
        pElemento = cFila.PegaProximo(TRUE);
        if(pElemento->m_Tipo != TIPO_ELEM_PREF)
        {
          bFull = TRUE;
          for(j = NUM_PROC_PREF; j < NUM_PROC; j++)
          {
            if(!cProcessador[j].Ocupado()) 
            {
              bFull = FALSE;
              break; // for()
            }
          }
          if(!bFull)  break; // while()
        }
        pElemento = cFila.AtendeProximo();
        cProcessador[i].Atende(pElemento, nClock);
      }

      for(; i < NUM_PROC; i++)
      {
        if(!cProcessador[j].Ocupado()) 
        {
          pElemento = cFila.AtendeProximo();
          cProcessador[i].Atende(pElemento, nClock);
        }
      }
    }

    for(i = 0; i < NUM_PROC; i++)
    {
      cProcessador[i].Atualiza(nClock);
    }

  }

  return 0;
}
