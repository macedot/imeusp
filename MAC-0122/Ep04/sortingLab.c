/***********************************************************
 * sortingLab.c
 * 
 * O objetivo deste exercicio programa e vericar experimentalmente e 
 * reportar os resultados das analises dos algoritmos de ordenacao apresentados em sala. 
 * Para tornar o experimento mais rico, faremos a ordenacao de vetores 
 * parcialmente ordenados de diferentes tamanhos.  
 * 
 ***********************************************************/

/***********************************************************
 * Includes da aplicacao
 **********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/***********************************************************
 * Defines internos
 **********************************************************/
#define CLOCK_PROC   (double)(1500.0 * 1000 * 1000) /* frequencia do seu processador (arredonde); Ex: 1500 Mhz; */

#ifndef MAX_PATH
#define MAX_PATH     255         /* Tamanho maximo dos nomes de arquivo  */
#endif

#ifndef RAND_MAX
#define RAND_MAX     0x7fffffff  /* Maximo para geracao de valores random */
#endif

/***********************************************************
 * Variaveis globais da Aplicacao
 **********************************************************/
const int nVetorTam[]  = { 1000, 5000, 10000, 20000, 30000, 40000, 50000, -1 };
const int nVetorPerc[] = {    5,   10,    20,    40,    60,    80,   100, -1 };

/***********************************************************
 * Funcoes Diversas da Aplicacao
 **********************************************************/
/*----------------------------------------------------------
- Atraves de um opcode dos processadores Pentium e posteriores,
obtemos o clock "decorrido" do processador, i.e, se temos um
processador de 100 Mhz, a cada segundo, incrementamos este
acumulador em 100 milhoes de unidades (!!!);
Se dispusermos de uma variavel de 64-bits:
  val_64 = ((unsigned long)high) | (((unsigned long)low)<<32); 
----------------------------------------------------------*/
void myClock(
  unsigned int* pLow,
  unsigned int* pHigh 
) /* Sem valor de retorno */
{
  unsigned int nHigh=0, nLow=0;
  
#ifdef _WIN32
  __asm
  {
    rdtsc
    mov nHigh, edx
    mov nLow, eax
  }
#else
  /* outros modos (MacOS e quaisquer outros OS's...) */
  __asm__ __volatile__("rdtsc" : "=a" (nLow), "=d" (nHigh));
#endif
  *pLow  = nLow;
  *pHigh = nHigh;
}
/*----------------------------------------------------------
int* fillVet(int* pVet,int nSize)
- Preenche um vetor tal que v[i] = i ;
- Retorna o mesmo ponteiro recebido;
----------------------------------------------------------*/
int* fillVet( /* Preenche um vetor  */
  int* pVet, /* vetor a ser preenchido */
  int nSize  /* Tamanho do vetor */
) /* Retorna um ponteiro par ao vetor alocado */
{
  register int i;

  if(pVet!=NULL)
  {
    for(i=0; i<nSize; i++)
    {
      pVet[i] = i;
    }
  }
  return(pVet);
}
/*----------------------------------------------------------
void embaralhaV(int V[], int n, int p)
- Embaralha "p porcento" de umd ado vetor de int V,
atraves do swap do conteudo das posicoes;
----------------------------------------------------------*/
void embaralhaV(  /* Embaralha um vetor */
  int V[],  /* Vetor de entrada e saida */
  int n,    /* Tamanho do vetor informado */
  int p     /* Porcentagem do vetor a ser em baralhada */
) /* Sem valor de retorno */
{
  int  nNumTrocas, nRandA, nRandB, nAux, nFimAux;
  int* pIdx; /* vetor de indices disponiveis */

  /* aloca o vetor de indices */
  pIdx = (int*)malloc(n*sizeof(int));
  /* quarda sempre a ultima posicao valida do vetor de indices */
  nFimAux = n-1;
  if(pIdx)
  {
    /* copia o conteudo para o vetor de indices */
    memcpy(pIdx,V,(n*sizeof(int)));
    /* calcula o numero de trocas aproximado para "embaralhar" o vetor */
    nNumTrocas = (int)((double)((n*p)/200.0));
    while((nNumTrocas>0)&&(nFimAux>0))
    {
      /* seleciona 2 indices aleatoriamente e faz o swap dos elementos nas respectivas posicoes */
      nRandA = (int)((double)((nFimAux*(double)rand()))/(RAND_MAX+1.0));
      nRandB = (int)((double)((nFimAux*(double)rand()))/(RAND_MAX+1.0));
      nAux            = V[pIdx[nRandA]];
      V[pIdx[nRandA]] = V[pIdx[nRandB]];
      V[pIdx[nRandB]] = nAux;
      /* atualiza o vetor de indices utilizaveis */
      pIdx[nRandA] = pIdx[nFimAux];
      nFimAux--;
      pIdx[nRandB] = pIdx[nFimAux];
      nFimAux--;
      /* decrementa o contador de trocas a serem realizadas */
      nNumTrocas--;
    }
    free(pIdx);
  }
}
/***********************************************************
 * Metodos de Ordenacao "Quadradicos"
 **********************************************************/
/*----------------------------------------------------------
void SelectionSort(int V[], int n)
- Metodo Selection Sort;
- Versao otimizada para minizar o numero de trocas;
----------------------------------------------------------*/
void SelectionSort( /* Ordena um vetor de int */
  int V[],  /* Vetor a ser ordenado */
  int n     /* Numero de elementos do vetor */
) /* Sem valor de retorno */
{
  register int i, j, nPos, nSwap; /* variaveis auxiliares */

  for(i=0; i<(n-1); i++)
  {
    nPos = i;
    nSwap = V[i];
    for(j=(i+1); j<n; j++)  
    {
      /* compara o valor atual com o valor da posicao armazenada */
      if(V[j]<nSwap)
      {
        nPos = j;
        nSwap = V[j];
      }
    }
    /* se ouve alteracao no indice do menor valor, realiza a troca */
    V[nPos] = V[i];
    V[i]    = nSwap;
  }
}
/*----------------------------------------------------------
void InsertionSort(int V[], int n)
- Metodo Insertion Sort
- Sem otimizacoes para este metodo
----------------------------------------------------------*/
void InsertionSort( /* Ordena um vetor de int */
  int V[],  /* Vetor a ser ordenado */
  int n     /* Numero de elementos do vetor */
) /* Sem valor de retorno */
{
  register int i, j, nSwap; /* variaveis auxiliares */

  for(i=1; i<n; i++)
  {
    for(j=i; ((j>0) && (V[j]<V[j-1])); j--)
    {
      nSwap  = V[j];
      V[j]   = V[j-1];
      V[j-1] = nSwap;
    }
  }
}
/*----------------------------------------------------------
void BubbleSort(int V[], int n)
- Metodo Bubble Sort
- Otimizado para interromper a busca, se nao houver nenhuma alteracao;
----------------------------------------------------------*/
void BubbleSort( /* Ordena um vetor de int */
  int V[],  /* Vetor a ser ordenado */
  int n     /* Numero de elementos do vetor */
) /* Sem valor de retorno */
{
  register int i, j, t, nSwap; /* variaveis auxiliares */

  for(i=(n-1); i>0; i--)
  {
    t=0;
    for(j=0; j<i; j++)
    {
      if(V[j]>V[j+1])
      {
        nSwap  = V[j];
        V[j]   = V[j+1];
        V[j+1] = nSwap;
        t++;
      }
    }
    /* Se nao houveram trocas, o vetor esta ordenado */
    if(t==0) break;
  }
}
/***********************************************************
 * Metodos de Ordenacao "Nao-quadraticos"
 **********************************************************/
/*----------------------------------------------------------
void Merge(int V[], int W[], int inicio, int meio, int fim)
- Metodo Merge utilizado em MergeSort (abaixo)
----------------------------------------------------------*/
void Merge( /* Mescla dois vetores, ordenando */
  int V[],    /* Vetor de entrada */
  int W[],    /* Vetor de saida */
  int inicio, /* Inicio do vetor da esquerda */
  int meio,   /* Fim do vetor da esquerda/Posicao anterior ao inicio do vetori da direita */
  int fim     /* Fim do vetor da direita */
) /* Sem valor de retorno */
{
  register int i, j, k;

  i = k = inicio;
  j = meio;
  /* percorre o vetor de entrada utilizando 2 pivots */
  while((i<meio) && (j<=fim))
  {
    /* copia o menor valor das atuais posicoes dos pivots para o vetor de resultado */
    if(V[i]<V[j])
    {
      W[k] = V[i];
      i++;
    }
    else
    {
      W[k] = V[j];
      j++;
    }
    k++;
  }
  /* se houver, copia restante do vetor da esquerda para o vetor de resultado; */
  while(i<meio)
  {
    W[k] = V[i];
    i++;
    k++;
  }
  /* se houver, copia restante do vetor da esquerda para o vetor de resultado; */
  while(j<=fim)
  {
    W[k] = V[j];
    j++;
    k++;
  }
  /* atualiza vetor de entrada */
  memcpy(V+inicio, W+inicio, (fim-inicio+1)*sizeof(int)) ;
}
/*----------------------------------------------------------
void MergeSort(int V[], int W[], int inicio, int fim)
- Metodo MergeSort
----------------------------------------------------------*/
void MergeSort( /* Ordena um vetor de int */
  int V[],     /* Vetor de entrada */
  int W[],     /* Vetor auxiliar */
  int inicio,  /* inicio do vetor a ser ordenado */
  int fim      /* final do vetor a ser ordenado */
) /* Sem valor de retorno */
{
  int nMeio;

  /* base da recursao eh o inicio do vetor ser igual ao final */
  if(inicio<fim)
  {
    /* passo da recursao consiste na divisao ao meio do vetor a ser ordenado */
    nMeio = (int)((double)((inicio + fim)/2.0));
    MergeSort(V, W, inicio, nMeio);
    MergeSort(V, W, nMeio+1, fim);
    /* Mescla os dois vetores, ordenando */
    Merge(V, W, inicio, nMeio+1, fim);
  }
}
/*----------------------------------------------------------
int particiona(int V[], int inicio, int fim)
- Metodo de Particionamento utilizando em QuickSort (abaixo)
----------------------------------------------------------*/
int particiona( /* Particiona um vetor para QuickSort */
  int V[],     /* Vetor a ser particionado */
  int inicio,  /* Inicio do vetor */
  int fim      /* Final do vetor */
) /* Retorna o indice "divisor de aguas" */
{
  register int i, j, nPivot, nAux;

  nPivot = V[fim];
  i = (inicio - 1);
  for(j=inicio; j<fim; j++)
  {
    if(V[j]<=nPivot)
    {
      i++;
      nAux = V[j];
      V[j] = V[i];
      V[i] = nAux;
    }
  }
  i++;
  V[fim] = V[i];
  V[i]   = nPivot;
  return(i);
}
/*----------------------------------------------------------
void QuickSort(int V[], int inicio, int fim)
- Metodo QuickSort
----------------------------------------------------------*/
void QuickSort( /* Ordena um vetor de int */
  int V[],     /* Vetor de entrada */
  int inicio,  /* inicio do vetor a ser ordenado */
  int fim      /* final do vetor a ser ordenado */
) /* Sem valor de retorno */
{
  int nMeio;

  /* base da recursao eh o inicio do vetor ser igual ao final */
  if(inicio<fim)
  {
    /* passo da recursao consiste no particionamento do vetor, ordenando um pivot */
    nMeio = particiona(V, inicio, fim);
    QuickSort(V, inicio, nMeio-1);
    QuickSort(V, nMeio+1, fim);
  }
}
/***********************************************************
 * Funcao principal
 **********************************************************/
/*----------------------------------------------------------
int main(int argc, char* argv[])
- Funcao principal do programa
----------------------------------------------------------*/
int main( /* Funcao principal */
  int argc,     /* Numero de paramentros na linha de comando */
  char* argv[]  /* Paramentros na linha de comando */
) /* Retorna um codigo de erro se houver; 0 se OK  */
{
  /* variaveis relacionadas a medicao dos tempos */
  unsigned int nLowA = 0, nHighA = 0;  /* contador de clock inicial */
  unsigned int nLowB = 0, nHighB = 0;  /* contador de clock final  */
  double       fTime   = 0.0;          /* diferença entre os contadores de clock dividido pela frequencia */
  /* variaveis locais gerais */
  register int i    = 0;           /* controle de loop geral */
  register int j    = 0;           /* controle de loop geral */
  errno_t  err;
  FILE*    fpArq     = NULL;        /* ponteiro para o arquivo de saida */
  int*     pVet      = NULL;        /* ponteiro para o vetor a ser ordenado ,*/
  int*     pVetAux   = NULL;        /* ponteiro para o vetor a ser ordenado ,*/
  int      nMax      = 0;           /* tamanho maximo a ser gerado */
  int      nSeed     = 0;           /* semente para geracao de numeros aleatorios */
  int      bOk       = 1;           /* flag de verificacao da passagem de parametros dna linha de comando */
  char     szMetodo[21];            /* metodo de ordenacao passado pela linha de  comando */
  char     szArqSaida[MAX_PATH+1];  /* arquivo de saida passado pela linha de  comando */


#ifndef __STDC__
  printf("*** Este programa nao foi gerado com padronizacao ANSI C ***\n");
#endif

  /* Trata os parametros da linha de comando; */
  for(i=1; i<argc; i++) 
  {
    if(argv[i][0] == '-')
    {
      switch(argv[i][1]) 
      {
        case 'm': strncpy_s(szMetodo,sizeof(szMetodo),argv[i+1],20);           break;
        case 'o': strncpy_s(szArqSaida,sizeof(szArqSaida),argv[i+1],MAX_PATH); break;
        case 's': nSeed = atoi(argv[i+1]);                break;
        default : bOk = 0;                                break;
      }
    }
  }
  /* Checa se os parametros foram informados corretamente; */
  bOk = (bOk && (argc==7));
  bOk = (bOk && (*szMetodo!='\0'));
  bOk = (bOk && (*szArqSaida!='\0'));
  if(!bOk) 
  { 
    if(argc!=1)
    {
      printf("Parametros na linha de comando nao sao validos! (%d)\n",argc);
    }
    printf("Utilize:\nsortingLab -m <metodo> -s <semente> -o <arquivo>\n");
    printf("\t <metodo> : insertion, selection, bubble, quick ou merge;\n");
    printf("\t <semente>: Semente para geracao dos vetores de teste e embaralhamento;\n");
    printf("\t <arquivo>: Arquivo de saida contendo os resultados;\n");
    return(21);
  }
  /* inicializa semente para geracao de numeros pseudo aleatorios */
  srand(nSeed);
  /* tenta abrir o arquivo de saida */
  err = fopen_s(&fpArq, szArqSaida,"w+b");
  if(err)
  {
    printf("Nao foi possivel criar o arquivo de saida: '%s';\n",szArqSaida);
    return(21);
  }
  /* capitaliza primeira letra e utiliza caixa baixa no restante do nome do metodo utilizado */
  /* mera estatica/rescura... */
  szMetodo[0] = (char)toupper(szMetodo[0]);
  for(i=1; (szMetodo[i]!='\0'); i++) 
  {
    szMetodo[i]=(char)tolower(szMetodo[i]);
  }
  fprintf(fpArq,"%-10s",szMetodo);
  /* percorre vetor de porcentagens de embaralhamento */
  for(j=0; (nVetorPerc[j]>0); j++)
  {
    fprintf(fpArq,"\t%d%%",nVetorPerc[j]);
  }
  fprintf(fpArq,"\n");
  fflush(fpArq);

  /* cria vetor auxiliar para merge */
  if(strcmp(szMetodo,"Merge")==0) 
  {
    nMax = 0;
    for(i=0; (nVetorTam[i]>0); i++)
    {
      nMax = ((nMax > nVetorTam[i]) ? nMax : nVetorTam[i]);
    }
    pVetAux = (int*)malloc(nMax*sizeof(int));
    if(pVetAux==NULL)
    {
      fclose(fpArq);
      printf("Nao foi possivel alocar o vetor auxiliar para Merge\n");
      return(11);
    }
  }
  /* percorre vetor de quantidades de elementos */
  for(i=0; (nVetorTam[i]>0); i++)
  {
    fprintf(fpArq,"%10d", nVetorTam[i]);
    /* cria um vetor do tamanho desejado */
    pVet = (int*)malloc(nVetorTam[i]*sizeof(int));
    if(pVet!=NULL)
    {
      /* percorre vetor de porcentagens de embaralhamento */
      for(j=0; (nVetorPerc[j]>0); j++)
      {
        fTime = 0.0;
        /* preenche o vetor tq v[i] = i e embaralha */
        fillVet(pVet, nVetorTam[i]);
        embaralhaV(pVet, nVetorTam[i], nVetorPerc[j]);
        nLowB = nLowA = nHighB = nHighA = 0;
        /* trata metodo de ordenacao selecioando */
        /* eh importande realizar as medidas dentro dos escopos dos ifs 
        para nao prejudicar metodos que estao "lah pra baixo" (paranoia, eu sei...) */
        if(strcmp(szMetodo,"Insertion")==0) 
        {
          myClock(&nLowA, &nHighA);
          InsertionSort(pVet, nVetorTam[i]);
          myClock(&nLowB, &nHighB);
        }
        else if(strcmp(szMetodo,"Selection")==0) 
        {
          myClock(&nLowA, &nHighA);
          SelectionSort(pVet, nVetorTam[i]);
          myClock(&nLowB, &nHighB);
        }
        else if(strcmp(szMetodo,"Bubble")==0) 
        {
          myClock(&nLowA, &nHighA);
          BubbleSort(pVet, nVetorTam[i]);
          myClock(&nLowB, &nHighB);
        }
        else if(strcmp(szMetodo,"Quick")==0) 
        {
          myClock(&nLowA, &nHighA);
          QuickSort(pVet, 0, nVetorTam[i]-1);
          myClock(&nLowB, &nHighB);
        }
        else if(strcmp(szMetodo,"Merge")==0) 
        {
          myClock(&nLowA, &nHighA);
          MergeSort(pVet, pVetAux, 0, nVetorTam[i]-1);
          myClock(&nLowB, &nHighB);
        }
        /* calcula tempo decorrido */
        fTime = ((double)((nLowB - nLowA) | ((nHighB - nHighA) << 31)) / CLOCK_PROC);
        /* fTime =  ((double)((long long)(nLowB-nLowA) | ((long long)(nHighB-nHighA) << 32)) / CLOCK_PROC); */
        fprintf(fpArq,"\t%02.8f",fTime);
      }
      free(pVet);
    }  
    fprintf(fpArq,"\n");
    fflush(fpArq);
  }
  fclose(fpArq);

  /* desaloca vetor auxiliar, se existir */
  if(pVetAux!=NULL) 
  {
    free(pVetAux);
  }
 
  return(0);
}

