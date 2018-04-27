/***********************************************************
Includes da aplicacao
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***********************************************************
Defines internos
***********************************************************/
/* Definicoes utilizadas nos retornos de funcoes */
#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

/* Tamanho maximo dos nomes de arquivo  */
#ifndef MAX_PATH
#define MAX_PATH     255
#endif

/* Dimensao maxima das matrizes utilizadas (ex: M[MAX_MAT][MAX_MAT] */
#define MAX_MAT      100

/* Tamanho maximo da linha de um arquivo */
#define MAX_LINHA    ((MAX_MAT*(1<<sizeof(double))))

/* Exemplo de uso corredo do programa */
#define PROG_USAGE   "Utilize:\nmatMult -A <matriz A> -B <matriz B> -o <matriz resultado>\n"

/* 
-> Se voce esta lendo isso, voce nao eh o corretor automatico,
   entao se quiser checar as mensages do EP, 
   descomente o define SHOW_MSG abaixo =D
*/
/* Define para exibicao de mensagens do programa */
/* #define SHOW_MSG */

/***********************************************************
Funcoes do programa
***********************************************************/
/*----------------------------------------------------------
void matmult(double A[][MAX_MAT], Primeira matriz de entrada 
             double B[][MAX_MAT], Segunda matriz de entrada 
             double C[][MAX_MAT], Matriz resultante 
             int m,               Numero de linhas da matriz A 
             int p,               Numero de colunas da matriz A/linhas da matriz B
             int n                Numero de colunas da matriz B 
)
3.1 A funcao matmult
Escreva uma funcao que multiplica duas matrizes do tipo double A[m X p] e B[p X n].
A constante MAX_MAT e definida no come¸co do programa (como 100).
Faz a multiplicacao de duas matrizes e guarda o resultado em C;
----------------------------------------------------------*/
void matmult(double A[][MAX_MAT],double B[][MAX_MAT],double C[][MAX_MAT],int m,int p,int n)
{
  register int i=0, j=0, k=0;

  /* Percorre linhas  */
  for(i=0; i<m; i++)
  {
    /* Percorre colunas */
    for(j=0; j<n; j++)
    {
      /* Acumula produtos A x B */
      C[i][j] = 0.0;
      for(k=0; k<p; k++)
      {
        C[i][j] += (A[i][k] * B[k][j]);
      }
    }
  }
}
/*----------------------------------------------------------
int leMatriz(double M[][MAX_MAT], Matriz de entrada
             int* pRows,          Ponteiro para o numero de linhas de M
             int* pCols,          Ponteiro para o numero de colunas de M
             char* szArquivo      Nome do arquivo da matriz
)
3.2 A funcao leMatriz
Escreva uma funcao que le uma matriz do tipo double A[m X p].
Faz a leitura de uma matriz guarda o resultado em M.
----------------------------------------------------------*/
int leMatriz(double M[][MAX_MAT], /* Matriz de entrada */
             int* pRows,          /* Ponteiro para o numero de linhas de M */
             int* pCols,          /* Ponteiro para o numero de colunas de M */
             char* szArquivo      /* Nome do arquivo da matriz */
)
{
  register int i=0, j=0;
  FILE* fpArq = NULL;
  char* pStr  = NULL;
  char  szBuffer[MAX_LINHA+2]; /* Buffer de leitura de linha: MAX_LINHA + CRLF */
  char  szToken[]=" ,\t";

  /* Inicializa tamanhos lidos para tratarmos externamente se houve leitura */
  *pRows = *pCols = 0;
  /* Abre arquivo de origem */
  fpArq = fopen(szArquivo,"rb");
  if(fpArq==NULL)
  {
    return(FALSE);
  }
  memset(szBuffer,0,MAX_LINHA+2);
  /* Le primeira linha: numero de linhas e colunas da matriz */
  fgets(szBuffer,MAX_LINHA,fpArq);
  pStr = strtok(szBuffer,szToken);
  *pRows = atoi(pStr);
  pStr = strtok(NULL,szToken);
  *pCols = atoi(pStr);
  /* Limita leitura dos parametros a MAX_MAT */
  *pRows = (*pRows<MAX_MAT ? *pRows : MAX_MAT);
  *pCols = (*pRows<MAX_MAT ? *pCols : MAX_MAT);
  i = 0;
  /* Percorre o arquivo e carrega a matriz em M */
  while((*pRows>i)&&(fgets(szBuffer,MAX_LINHA,fpArq)!=NULL))
  {
    pStr = strtok(szBuffer,szToken);
    j = 0;
    while((*pCols>j)&&(pStr!=NULL))
    {
      M[i][j] = atof(pStr);
      pStr = strtok(NULL,szToken);
      j++;
    }
    i++;
  }
  /* Fecha o arquivo de origem */
  fclose(fpArq);
  fpArq=NULL;
  return(TRUE);
}
/*----------------------------------------------------------
3.3 A funcao escreveMatriz
Escreva uma funcao que escreve uma matrizes do tipo double A[m X p] com o mesmo formato da
matriz de entrada. A funcao deve usar, obrigatoriamente, a assinatura abaixo:
----------------------------------------------------------*/
int escreveMatriz(double M[][MAX_MAT], /* Matriz de entrada */
                  int* pRows,          /* Ponteiro para o numero de linhas de M */
                  int* pCols,          /* Ponteiro para o numero de colunas de M */
                  char* szArquivo      /* Nome do arquivo da matriz */
)
{
  register int i=0, j=0;
  FILE* fpArq=NULL;

  /* Tenta criar/abrir o arquivo de destino */
  fpArq=fopen(szArquivo,"w+b");
  if(fpArq==NULL)
  {
#ifdef SHOW_MSG
    printf("Nao foi possivel criar o arquivo '%s'.\n",szArquivo);
#endif    
    return(FALSE);
  }
  /* Gravamos na primeira linha o numero de linhas e colunas da matriz */
  fprintf(fpArq,"%d\t%d\r\n",*pRows,*pCols);
  fflush(fpArq);
  /* Gravamos a matriz, separando as colunas com '\t' */
  for(i=0; i<*pRows; i++)
  {
    fprintf(fpArq,"%0.2f",M[i][0]);
    for(j=1; j<*pCols; j++)
    {
      fprintf(fpArq,"\t%0.2f",M[i][j]);
    }
    fprintf(fpArq,"\r\n");
    fflush(fpArq);
  }
  /* Fecha o arquivo de destino */
  fclose(fpArq);
  fpArq=NULL;
  return(TRUE);
}
/*----------------------------------------------------------
3.4 A funcao main
Escreva uma funcao principal que trata os argumentos de entrada, 
chama a funcao que le as duas matrizes que serao multiplicadas, 
verifica se as matrizes podem ser multiplicadas, chama a funcao 
que multiplica as matrizes e, finalmente, chama a funcao que 
escreve a matriz resultante.
----------------------------------------------------------*/
int main(int argc, char* argv[]) 
{
  double A[MAX_MAT][MAX_MAT];     /* Matriz de Entrada 1 */
  double B[MAX_MAT][MAX_MAT];     /* Matriz de Entrada 2 */
  double C[MAX_MAT][MAX_MAT];     /* Matriz de Saida */
  char   szArqMatriz1[MAX_PATH];  /* Path para o arquivo de entrada 1 */
  char   szArqMatriz2[MAX_PATH];  /* Path para o arquivo de entrada 2 */
  char   szArqSaida[MAX_PATH];    /* Path para o arquivo de saida */
  int    i=0, bOk=1;              /* Controle de Loop e flags */
  int    nRowsA=0,nRowsB=0;       /* Numero de linhas */
  int    nColsA=0,nColsB=0;       /* Numero de colunas */

  printf("%d\n",MAX_LINHA);
  /* Verificacao simples, a chamada deve ter 7 argumentos */
  if(argc!=7) 
  { 
#ifdef SHOW_MSG
    printf(PROG_USAGE);
#endif    
    /* Finaliza o programa com falha */
    return(21);
  }
  /* Inicializa primeira posicao dos buffers */
  *szArqMatriz1=*szArqMatriz2=*szArqSaida='\0';
  /* Trata os parametros da linha de comando */
  for(i=1; i<7; i++) 
  {
    if(argv[i][0] == '-')
    {
      switch(argv[i][1]) 
      {
        case 'A': strcpy(szArqMatriz1,argv[i+1]);  break ;
        case 'B': strcpy(szArqMatriz2,argv[i+1]);  break ;
        case 'o': strcpy(szArqSaida,argv[i+1]) ;   break ;
        default : bOk=0; break ;
      }
    }
  }
  /* Checa se os parametros foram informados corretamente */
  bOk = (bOk && (*szArqMatriz1!='\0') && (*szArqMatriz2!='\0') && (*szArqSaida!='\0'));
  if(!bOk) 
  {
#ifdef SHOW_MSG
    printf("Parametros na linha de comando nao sao validos!\n");
    printf(PROG_USAGE);
#endif    
    return(21);
  }
  /* Le arquivos de matrizes para buffers em memoria; */
  if(!leMatriz(A,&nRowsA,&nColsA,szArqMatriz1))
  {
#ifdef SHOW_MSG
    printf("Nao foi possivel ler o arquivo da matriz A: '%s'\n",szArqMatriz1);
#endif    
    return(1);
  }
  if(!leMatriz(B,&nRowsB,&nColsB,szArqMatriz2))
  {
#ifdef SHOW_MSG
    printf("Nao foi possivel ler o arquivo da matriz B: '%s'\n",szArqMatriz2);
#endif    
    return(1);
  }
  /* Verifica se as matrizes sao compativeis */
  if(nColsA!=nRowsB) 
  {
#ifdef SHOW_MSG
    printf("Matrizes informadas possuem dimensoes incompativeis:\n");
    printf("A[%d x %d]\nB[%d x %d]\n",nRowsA,nColsA,nRowsB,nColsB);
#endif    
    return(34);
  }
  /* Realiza a devida multiplicacao das matrizes; */
  matmult(A,B,C,nRowsA,nColsA,nColsB);
  /* Grava matriz de resultados; */
  if(!escreveMatriz(C,&nRowsA,&nColsB,szArqSaida))
  {
#ifdef SHOW_MSG
    printf("Nao foi possivel gravar o arquivo da matriz de resultado: '%s'\n",szArqSaida);
#endif    
    return(1);
  }
  /* Finaliza o programa com sucesso */
  return(0);
} 
