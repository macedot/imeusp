/***********************************************************
Includes da aplicacao
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/***********************************************************
Defines internos
***********************************************************/
/* Definicoes utilizadas nos retornos de funcoes */
#ifndef TRUE
#define TRUE         1
#endif

#ifndef FALSE
#define FALSE        0
#endif

/* Tamanho maximo dos nomes de arquivo  */
#ifndef MAX_PATH
#define MAX_PATH     255
#endif

/* podemos querer usar um sistema não decimal, que tal? =D */
#define BASE_NUM     10

/* exemplo de uso do programa */
#define PROG_USAGE   "Utilize:\ncalculadora -m <operacao> -a <arquivo1> -b <arquivo2> -o <arquivoResultado>\n"

/* liga ou desliga o modo de profile */
#define MAX_PROFILE  20

/***********************************************************
Definicao de tipos internos da aplicacao
***********************************************************/
typedef struct elem {
  int digito ;
  struct elem* prox ;
} Elem ;

/***********************************************************
Funcoes diversas
***********************************************************/
/*----------------------------------------------------------
Elem* criaElem(int nValor)
- Aloca um elemento Elem, inicializado com 'nValor';
----------------------------------------------------------*/
Elem* criaElem(int nValor)
{
  Elem* pElem=NULL;
  
  /* aloca memoria para um novo container; invalido se NULL */
  if((pElem=(Elem*)malloc(sizeof(Elem)))!=NULL)
  {
    pElem->digito=nValor;
    pElem->prox=NULL;
  }
  return(pElem); 
}
/*----------------------------------------------------------
Elem* readFile2Lista_Rev(char* szArquivo)
- Le o arquivo informado, e retorna uma lista ligada para
seu conteudo; primeiro digito do arquivo eh o ultimo da lista;
----------------------------------------------------------*/
Elem* readFile2Lista_Rev(char* szArquivo)
{
  Elem* pAnterior = NULL;
  Elem* pAtual    = NULL;
  FILE* fpArq     = NULL;
  int   nRead     = 0;

  /* abre um arquivo jah existente em modo binario */
  if((fpArq = fopen(szArquivo,"rb"))==NULL) return(pAtual);
  /* percorre ate fim de arquivo */
  while(!feof(fpArq))
  {
    /* na leitura digito-a-digito, verifica se os caracteres sao validos */
    nRead = fgetc(fpArq);
    if(isdigit(nRead))
    {
      /* converte o caracter lido para o digito correspondente */
      nRead -= '0';
      /* cria novo container  para o digito  */
      pAtual = criaElem(nRead);
      /* faz com que o container tenha como proximo o elemento lido anteriormente */
      pAtual->prox = pAnterior;
      /* atualiza elemento atual como o container criado */
      pAnterior = pAtual;
    }
  }
  fclose(fpArq);  fpArq=NULL;
  return(pAtual);
}
/*----------------------------------------------------------
int writeLista2File(char* szArquivo,Elem* pLista)
- Grava uma lista ligada no arquivo informado;
----------------------------------------------------------*/
int writeLista2File(char* szArquivo,Elem* pLista)
{
  Elem* pAtual = NULL;
  FILE* fpArq  = NULL;
  int   nAux   = 0;

  /* abre um arquivo para escrita; cria se nao existir; */
  if((fpArq = fopen(szArquivo,"wb"))==NULL) return(0);
  /* percorre a lista dada, convertendo os digitos em caracteres ASCII */
  pAtual = pLista;
  while(pAtual!=NULL)
  {
    nAux = pAtual->digito+'0';
    fputc(nAux,fpArq);
    pAtual = pAtual->prox;
  }
  /* padrao ANSI C: newline antes do fim do arquivo (nao eh brincadeira!) */
  fputc('\n',fpArq);
  fclose(fpArq);  fpArq=NULL;
  return(1);
}
/*----------------------------------------------------------
void freeLista(Elem* pLista)
Libera memoria utilizada por uma lista ligada
----------------------------------------------------------*/
void freeLista(Elem* pLista)
{
  Elem* pAtu=NULL;
  Elem* pAux=NULL;

  pAtu = pLista;
  while(pAtu!=NULL)
  {
    pAux = pAtu->prox;
    free(pAtu);
    pAtu = pAux;
  }
}
/*----------------------------------------------------------
Elem* getLastElem(Elem* pLista)
- Retorna um ponteiro para o ultimo elemento valido de uma lista ligada
----------------------------------------------------------*/
Elem* getLastElem(Elem* pLista)
{
  Elem* pAux = pLista;

  /* equanto o item atual e o proximo forem validos, percorremos a lista */
  while(pAux!=NULL && pAux->prox!=NULL) pAux = pAux->prox;
  return(pAux);
}
/*----------------------------------------------------------
Elem* apendElem(Elem* pLista,int nValor)
- Concatena um novo elemento a uma lista ligada
----------------------------------------------------------*/
Elem* apendElem(Elem* pLista,Elem* pNew)
{
  Elem* pRet = pNew;
  Elem* pAux = NULL;

  /* pega o ultimo item da lista; invalido se for NULL */
  if((pAux = getLastElem(pLista))!=NULL)
  {
    pAux->prox = pNew;
    pRet = pLista;
  }
  return(pRet);
}
/*----------------------------------------------------------
Elem* inverteLista(Elem* pLista);
- Inverte uma lista ligada, retornando seu novo inicio
----------------------------------------------------------*/
Elem* inverteLista(Elem* pLista)
{
  Elem* pAnterior = NULL;
  Elem* pAtual    = NULL;
  Elem* pProximo  = NULL;

  /* para os casos triviais, tratamos em separado */
  if(pLista==NULL) return(NULL);
  if(pLista->prox==NULL) return(pLista);
  /* item atual eh o segundo da lista */
  pAtual = pLista->prox;
  pAnterior = pLista;
  pAnterior->prox = NULL;
  while(pAtual!=NULL)
  {
    pProximo = pAtual->prox; 
    pAtual->prox = pAnterior;
    pAnterior = pAtual;
    pAtual = pProximo; 
  }  
  return(pAnterior);
}
/***********************************************************
Funcao principal
***********************************************************/
/*----------------------------------------------------------
int main(int argc, char* argv[]) 
----------------------------------------------------------*/
int main(int argc, char* argv[]) 
{
  register int i = 0;           /* controle de loop geral */
  Elem* pLista  = NULL;        /* lista ligada contendo o valor 'revertido' do arquivo A */
  char  szArquivoA[MAX_PATH+1]; /* path para o arquivo A */
  int   bOk=TRUE;               /* flag de retorno */
  time_t  tTime = 0;
  time_t  tTotalTime = 0;

#ifndef __STDC__
  printf("Este programa nao foi gerado com padronizacao ANSI C!\n");
#endif

  /* Verificacao simples, a chamada deve ter 9 argumentos; */
  if(argc!=9) 
  { 
    if(argc!=1)
    {
      printf("Parametros na linha de comando nao sao validos! (%d)\n",argc);
    }
    printf(PROG_USAGE);
    return(21);
  }
  /* inicializa path dos arquivos */
  *szArquivoA = '\0';
  /* Trata os parametros da linha de comando; */
  for(i=1; i<argc; i++) 
  {
    if(argv[i][0] == '-')
    {
      switch(argv[i][1]) 
      {
        case 'a': strncpy(szArquivoA,argv[i+1],MAX_PATH); break;
      }
    }
  }
  /* Checa se os parametros foram informados corretamente; */
  bOk = (bOk && (*szArquivoA!='\0'));
  if(!bOk) 
  {
    printf("Inconcistencia na passagem de parametros na linha de comando!\n");
    printf(PROG_USAGE);
    return(34);
  }

  tTotalTime = 0;
  for(i=0; i<MAX_PROFILE; i++) 
  {
    /* le dos arquivos de entrada os numeros, convertidos em lista ligada, em ordem reversa */
    pLista = readFile2Lista_Rev(szArquivoA);
    if(pLista==NULL)
    {
      printf("Erro ao abrir o arquivo de entrada '%s'.\n",szArquivoA);
      return(11);
    }
    tTime = time(NULL);
    pLista = inverteLista(pLista);
    tTime = (time(NULL) - tTime);
    freeLista(pLista);
    tTotalTime += tTime;
    printf("Profile %d: %d (s)\n",i,tTime);
  } /* for(i=0; i<MAX_PROFILE; i++) */
  printf("Geral %d (s)\n",tTotalTime);
  return(0);
}

