/***********************************************************
Defines para profile interno da aplicaocao
***********************************************************/
/* liga ou desliga o modo de profile */
#define PROFILE_APP
#define MAX_PROFILE  1

/***********************************************************
Includes da aplicacao
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef PROFILE_APP
#include <time.h>
#endif /* PROFILE_APP */

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

/* define para debug da aplicacao */
/* #define DEBUG_PROG */

/***********************************************************
Definicao de tipos internos da aplicacao
***********************************************************/
typedef struct elem {
  int digito ;
  struct elem* prox ;
} Elem ;

/***********************************************************
Variaveis globais internas
***********************************************************/
/* ponteiro para funcao da operacao desejada */
Elem* (*funcOper)(Elem* pValorA,Elem* pValorB);

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
void freeLista(Elem* pLista)
- Libera memoria utilizada por uma lista ligada
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
  int   bZero     = 0;

  /* abre um arquivo jah existente em modo binario */
  if((fpArq = fopen(szArquivo,"rb"))==NULL) return(pAtual);
  /* adendo: ignora zeros no inicio o arquivo */
  while(!feof(fpArq))
  {
    /* na leitura digito-a-digito, verifica se os caracteres sao validos */
    nRead = fgetc(fpArq);
    if(isdigit(nRead))
    {
      /* verifica se o arquivo comeca com zeros */
      bZero = (nRead=='0');
      if(!bZero)
      {
        bZero = 0;
        /* converte o caracter lido para o digito correspondente */
        nRead -= '0';
        /* cria novo container  para o digito  */
        pAtual = criaElem(nRead);
        /* faz com que o container tenha como proximo o elemento lido anteriormente */
        pAtual->prox = pAnterior;
        /* atualiza elemento atual como o container criado */
        pAnterior = pAtual;
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
        nRead = 0;
        /* com certeza, tivemos ao menos um digito valido! */
        break; /* while(!feof(fpArq)) (1) */
      }
    }
  }
  /* verifica se o arquivo continha apenas zeros */
  if(bZero)
  {
    pAtual = criaElem(0);
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
  /* pula zeros no comeco do numero  */
  pAtual = pLista;
  while(pAtual!=NULL)
  {
    if(pAtual->digito!=0) break;
    pAtual = pAtual->prox;
  }
  /* verifica se estamos no fim da lista */
  if(pAtual==NULL)
  {
    /* so haviam zeros, ou lista vazia, grava um zero no arquivo */
    fputc('0',fpArq);
  }
  else
  {
    /* percorre a lista dada, convertendo os digitos em caracteres ASCII */
    while(pAtual!=NULL)
    {
      nAux = (pAtual->digito + '0');
      fputc(nAux,fpArq);
      pAtual = pAtual->prox;
    }
    /* padrao ANSI C: newline antes do fim do arquivo (nao eh brincadeira!) */
    /* removido para seguir padrao descrito no moodle; */
    /* fputc('\n',fpArq); */
  }
  fclose(fpArq);  fpArq=NULL;
  return(1);
}
/*----------------------------------------------------------
Elem* getLastElem(Elem* pLista)
- Retorna um ponteiro para o ultimo elemento valido de uma lista ligada
----------------------------------------------------------*/
Elem* getLastElem(Elem* pLista)
{
  Elem* pAux = pLista;

  /* verifica se eh uma lista valida */
  if(pAux!=NULL)
  {
    /* equanto o proximo for valido, percorremos a lista */
    while(pAux->prox!=NULL)  pAux = pAux->prox;
  }
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
/*----------------------------------------------------------
int printLista(Elem* pLista)
- Imprime uma dada lista ligada na saida padrao;
----------------------------------------------------------*/
#ifdef DEBUG_PROG
int printLista(Elem* pLista)
{
  register int nSize = 0;
  Elem* pAtu  = NULL;

  pAtu = pLista;
  while(pAtu!=NULL)
  {
    printf("%d",pAtu->digito);
    pAtu = pAtu->prox;
    nSize++;
  }
  printf("\n");
  return(nSize);
}
#endif /* DEBUG_PROG */
/***********************************************************
Funcoes de Operacao da Calculadora
***********************************************************/
/*----------------------------------------------------------
Elem* somaLista(Elem* pValorA,Elem* pValorB)
- Retorna uma lista ligada da soma das listas informadas
- NAO OTIMIZADA
----------------------------------------------------------*/
Elem* somaLista(Elem* pValorA,Elem* pValorB)
{
  Elem* pPvtA    = NULL; /* elemento atual no valor A */
  Elem* pPvtB    = NULL; /* elemento atual no valor B */ 
  Elem* pSoma    = NULL; /* elemento atual no valor B */ 
  Elem* pSomaAtu = NULL; /* elemento atual no valor B */ 
  Elem* pSomaAux = NULL; /* elemento atual no valor B */ 
  int   nCarry   = 0;
  int   nSoma    = 0;

#ifdef DEBUG_PROG
  printf("somaLista(%X,%X);\n",pValorA,pValorB);
#endif /* DEBUG_PROG */

  /* inicializacao dos pivots */
  pPvtA = pValorA;
  pPvtB = pValorB;
  /* se ambos forem validos... */
  if(pPvtA!=NULL && pPvtB!=NULL)
  {
    /* faz uma primeira pra inicializar os ponteiros */
    nSoma  = (pPvtA->digito + pPvtB->digito);
    nCarry = nSoma/BASE_NUM;
    nSoma -= nCarry*BASE_NUM;
    /* inicializa ponteiros da lista de soma */
    pSoma = pSomaAtu = criaElem(nSoma);
    /* avanca pivots */
    pPvtA = pPvtA->prox;
    pPvtB = pPvtB->prox;
    /* enquanto AMBOS forem validos */
    while(pPvtA!=NULL && pPvtB!=NULL)
    {
      /* soma digitos + vai um  */
      nSoma  = nCarry + (pPvtA->digito + pPvtB->digito);
      nCarry = nSoma/BASE_NUM;
      nSoma -= nCarry*BASE_NUM;
      /* gruda novo elemento na lista e avanca o pivot para ele */
      pSomaAtu->prox = criaElem(nSoma);
      pSomaAtu = pSomaAtu->prox;
      /* avanca pivots */
      pPvtA = pPvtA->prox;
      pPvtB = pPvtB->prox;
    }
  }
  /* se sobrou em A, ou se somente A eh valido */
  if(pPvtA!=NULL)
  {
    /* soma digito do valor A + vai um */
    nSoma  = (nCarry + pPvtA->digito);
    nCarry = nSoma/BASE_NUM;
    nSoma -= nCarry*BASE_NUM;
    pSomaAux = criaElem(nSoma);
    /* se ainda nao somou ninguem... */
    if(pSoma==NULL)
    {
      /* soma e atual sao o elemento criado */
      pSoma = pSomaAtu = pSomaAux;
    }
    else
    {
      /* se nao, gruda novo elemento na lista e avanca o pivot para ele */
      pSomaAtu->prox = pSomaAux;
      pSomaAtu = pSomaAtu->prox;
    }
    /* avanca pivot de A */
    pPvtA = pPvtA->prox;
    while(pPvtA!=NULL)
    {
      /* soma carry ao digito (semelhante ao loop acima) */
      nSoma  = (nCarry + pPvtA->digito);
      nCarry = nSoma/BASE_NUM;
      nSoma -= nCarry*BASE_NUM;
      /* novo proximo item sera um item novo (vixe!) */
      pSomaAtu->prox = criaElem(nSoma);
      /* atualiza item atual como o novo item criado */
      pSomaAtu = pSomaAtu->prox;
      /* avanca pivot de B */
      pPvtA = pPvtA->prox;
    }
  }
  else if(pPvtB!=NULL)
  {
    /* entao, ou sobrou em B, ou se somente B eh valido */
    /* soma digito do valor B + vai um */
    nSoma  = (nCarry + pPvtB->digito);
    nCarry = nSoma/BASE_NUM;
    nSoma -= nCarry*BASE_NUM;
    pSomaAux = criaElem(nSoma);
    /* se ainda nao somou ninguem... */
    if(pSoma==NULL)
    {
      /* soma e atual sao o elemento criado */
      pSoma = pSomaAtu = pSomaAux;
    }
    else
    {
      /* se nao, gruda o novo elemento na lista soma */
      pSomaAtu->prox = pSomaAux;
      pSomaAtu = pSomaAtu->prox;
    }
    /* avanca pivot de B */
    pPvtB = pPvtB->prox;
    while(pPvtB!=NULL)
    {
      /* soma carry ao digito (semelhante ao loop acima) */
      nSoma  = (nCarry + pPvtB->digito);
      nCarry = nSoma/BASE_NUM;
      nSoma -= nCarry*BASE_NUM;
      /* novo proximo item sera um item novo (vixe!) */
      pSomaAtu->prox = criaElem(nSoma);
      /* atualiza item atual como o novo item criado */
      pSomaAtu = pSomaAtu->prox;
      /* avanca pivot de B */
      pPvtB = pPvtB->prox;
    }
  }
  /* se vai um, eh pq nao foi um, entao vai um (O.o) */
  if(nCarry>0)
  {
    pSomaAtu->prox = criaElem(nCarry);
    pSomaAtu = pSomaAtu->prox;
  }
  /* se nao foi nada, entao vai pro inf.... zero! (^.^) */
  if(pSoma==NULL)
  {
    /* NOTA: Esta situacao nao eh prevista na especificacao */
    pSoma = criaElem(0);
  }
  return(pSoma);
}
/*----------------------------------------------------------
Elem* multiplicaLista(Elem* pValorA,Elem* pValorB)
- Retorna uma lista ligada do produto das listas informadas
- VERSAO NAO OTIMIZADA
----------------------------------------------------------*/
Elem* multiplicaLista(Elem* pValorA,Elem* pValorB)
{
  Elem* pAux         = NULL; /* container auxiliar */
  Elem* pPvtA        = NULL; /* elemento atual no valor A */
  Elem* pPvtB        = NULL; /* elemento atual no valor B */ 
  Elem* pSubProd     = NULL; /* produto de pPvtA->digito por pValorB com paddind de zeros */
  Elem* pSubProdIni  = NULL; /* primeiro elemento do subproduto descontados o padding de zeros */
  Elem* pSubProdAtu  = NULL; /* pivot de pSubProd */
  Elem* pListaZero   = NULL; /* lista com padding de zeros para o subproduto atual */
  Elem* pLastZero    = NULL; /* lista com padding de zeros para o subproduto atual */
  Elem* pProdFinal   = NULL; /* produto final entre pValorA e pValorB (acumulador de pSubProd) */
  int   nProd        = 0;    /* produto dos digitos atuais */
  int   nCarry       = 0;    /* valor multiplo da base que excede um digito da base */
  int   nPos         = 0;    /* posicao atual na lista A */

#ifdef DEBUG_PROG
  printf("multiplicaLista(%X,%X);\n",pValorA,pValorB);
#endif /* DEBUG_PROG */

  /* aborta se fator B for invalido  */
  if(pValorB==NULL) return(NULL);
  /* percorre fator A */
  pPvtA = pValorA;
  while(pPvtA!=NULL)
  {
#ifdef DEBUG_PROG
    printf("(%d)\n",nPos);
#endif /* DEBUG_PROG */

    /* se o digito atual do falot A for zero, 
    junta mais um zero na lista de zero e vai para o proximo digito de A */
    if(pPvtA->digito==0)
    {
      if(pLastZero!=NULL) pLastZero->prox = NULL;
      /* adiciona um zero na lista de zeros */
      pAux = criaElem(0);
      pListaZero = apendElem(pListaZero,pAux);
      /* avanca pivot de A */
      pPvtA = pPvtA->prox;
      nPos++;
      continue;
    }
    /* inicializa valores de controle */
    pPvtB = pValorB;
    pSubProd = NULL;
    nCarry = 0;
    /* produto do primeiro digito eh externo, para inicializar a lista */
    nProd = (pPvtA->digito * pPvtB->digito);
    nCarry = nProd/BASE_NUM;
    nProd -= (nCarry*BASE_NUM);
    /* avanca pivit de B */
    pPvtB = pPvtB->prox;
    pSubProdIni = pSubProdAtu = criaElem(nProd);
    
#ifdef DEBUG_PROG
    printf("pListaZero=");
    printLista(pListaZero);
#endif /* DEBUG_PROG */
    
    /* junta lista de zeros, se houver, com inicio do subproduto */
    pLastZero = getLastElem(pListaZero);
    pSubProd = apendElem(pListaZero,pSubProdIni);

#ifdef DEBUG_PROG
    printf("pSubProd=");
    printLista(pSubProd);
#endif /* DEBUG_PROG */

    /* percorre fator B */
    while(pPvtB!=NULL)
    {
      /* realiza demais produtos dos digitos */
      nProd = nCarry + (pPvtA->digito * pPvtB->digito);
      nCarry = nProd/BASE_NUM;
      nProd -= nCarry*BASE_NUM;
      /* avanca pivot do fator B */
      pPvtB = pPvtB->prox;
      /* armazena um novo elemento no final do subproduto */
      pSubProdAtu->prox = criaElem(nProd);
      pSubProdAtu = pSubProdAtu->prox;
    }
    /* sobrou um, vai mais um! */
    if(nCarry>0)
    {
      pSubProdAtu->prox = criaElem(nCarry);
      pSubProdAtu = pSubProdAtu->prox;
    }
    
#ifdef DEBUG_PROG
    printf("pSubProd=");
    printLista(pSubProd);
#endif /* DEBUG_PROG */

    /* acumula subproduto COM ZEROS A DIREITA no produto final */
    pProdFinal = somaLista(pProdFinal,pSubProd);

    /* libera lista utilizada pelo subproduto SEM ZEROS A DIREITA */
    freeLista(pSubProdIni); pSubProdIni=NULL;
    pPvtA = pPvtA->prox;
    nPos++;

    /* adiciona um zero na lista de padding */
    if(pLastZero!=NULL) pLastZero->prox = NULL;
    pAux = criaElem(0);
    pListaZero = apendElem(pListaZero,pAux);
    pLastZero = getLastElem(pListaZero);
  }
  /* se houve append de zeros, libera a lista */
  if(pListaZero!=NULL)
  {
    if(pLastZero!=NULL) pLastZero->prox = NULL;
    freeLista(pListaZero); 
    pListaZero=pLastZero=NULL;
  }
  /* opa! sacaneou e mandou zero!! */
  if(pProdFinal==NULL)
  {
    pProdFinal = criaElem(0);
  }
  return(pProdFinal);
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
  Elem* pValorA  = NULL;        /* lista ligada contendo o valor 'revertido' do arquivo A */
  Elem* pValorB  = NULL;        /* lista ligada contendo o valor 'revertido' do arquivo A */
  Elem* pResult  = NULL;        /* lista ligada contendo o valor final da operacao */
  char  szArquivoA[MAX_PATH+1]; /* path para o arquivo A */
  char  szArquivoB[MAX_PATH+1]; /* path para o arquivo B */
  char  szArqSaida[MAX_PATH+1]; /* path para o arquivo de saida */
  int   nOper=0;                /* numero da opcao desejada */
  int   bOk=TRUE;               /* flag de retorno */
#ifdef PROFILE_APP
  time_t  tTime = 0;
  time_t  tTotalTime = 0;
#endif /* PROFILE_APP */

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
  *szArquivoA = *szArquivoB = *szArqSaida = '\0';
  /* inicializa funcoes de processamento */
  funcOper = NULL;
  /* Trata os parametros da linha de comando; */
  for(i=1; i<argc; i++) 
  {
    if(argv[i][0] == '-')
    {
      switch(argv[i][1]) 
      {
        case 'm': nOper = atoi(argv[i+1]);                break;
        case 'a': strncpy(szArquivoA,argv[i+1],MAX_PATH); break;
        case 'b': strncpy(szArquivoB,argv[i+1],MAX_PATH); break;
        case 'o': strncpy(szArqSaida,argv[i+1],MAX_PATH); break;
        default : bOk=FALSE;                              break;
      }
    }
  }
  /* Checa se os parametros foram informados corretamente; */
  bOk = (bOk && (nOper>=1) && (nOper<=2));
  bOk = (bOk && (*szArquivoA!='\0'));
  bOk = (bOk && (*szArquivoB!='\0'));
  bOk = (bOk && (*szArqSaida!='\0'));
  if(!bOk) 
  {
    printf("Inconcistencia na passagem de parametros na linha de comando!\n");
    printf(PROG_USAGE);
    return(34);
  }
  

#ifdef PROFILE_APP
  tTotalTime = 0;
  for(i=0; i<MAX_PROFILE; i++) 
  {
#endif /* PROFILE_APP */

    /* le dos arquivos de entrada os numeros, convertidos em lista ligada, em ordem reversa */
    pValorA = readFile2Lista_Rev(szArquivoA);
    if(pValorA==NULL)
    {
      printf("Erro ao abrir o arquivo de entrada '%s'.\n",szArquivoA);
      return(11);
    }

#ifdef DEBUG_PROG
    printf("%s:\n",szArquivoA);
    printLista(pValorA);
#endif /* DEBUG_PROG */

    pValorB = readFile2Lista_Rev(szArquivoB);
    if(pValorB==NULL)
    {
      freeLista(pValorA);
      printf("Erro ao abrir o arquivo de entrada '%s'.\n",szArquivoB);
      return(12);
    }

#ifdef DEBUG_PROG
    printf("%s:\n",szArquivoB);
    printLista(pValorB);
#endif /* DEBUG_PROG */

    switch(nOper)
    {
      case 1: funcOper = somaLista;       break;
      case 2: funcOper = multiplicaLista; break;
    }

#ifdef PROFILE_APP
    tTime = time(NULL);
#endif /* PROFILE_APP */
    /* executa a operacao designada */
    pResult = funcOper(pValorA,pValorB);
#ifdef PROFILE_APP
    tTime = (time(NULL) - tTime);
    tTotalTime += tTime;
#endif /* PROFILE_APP */

    if(pResult!=NULL)
    {
      pResult = inverteLista(pResult);

#ifdef DEBUG_PROG
      printf("%s:\n",szArqSaida);
      printLista(pResult);
#endif /* DEBUG_PROG */

      bOk = writeLista2File(szArqSaida,pResult);
      freeLista(pResult);

      if(!bOk)
      {
        printf("Erro gravando o arquivo de destino '%s'.\n",szArqSaida);
        return(13);
      }
    }

    freeLista(pValorB);
    freeLista(pValorA);

#ifdef PROFILE_APP
    printf("Profile %d: %d (s)\n",i,tTime);
  } /* for(i=0; i<MAX_PROFILE; i++) */
  printf("Geral %d (s)\n",tTotalTime);
#endif /* PROFILE_APP */

  return(0);
}

