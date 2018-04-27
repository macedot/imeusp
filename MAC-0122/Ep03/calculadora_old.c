/***********************************************************
Includes da aplicacao
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/***********************************************************
Defines internos
***********************************************************/
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
        pAtual->prox = pAnterior;
        /* atualiza elemento anterior como o container criado */
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
            pAtual->prox = pAnterior;
            /* atualiza elemento anterior como o container criado */
            pAnterior = pAtual;
          }
        }
        nRead = 0;
        /* com certeza, tivemos ao menos um digito valido! */
        break; /* while(!feof(fpArq)) (1) */
      }
    }
  }
  /* fecha o arquivo; nao precisamos mais dele,,,, xô, xô! */
  fclose(fpArq);  fpArq=NULL;
  /* verifica se o arquivo continha apenas zeros */
  if(bZero)
  {
    pAtual = criaElem(0);
  }
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
  if((fpArq = fopen(szArquivo,"wba"))==NULL) return(0);
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
----------------------------------------------------------*/
Elem* somaLista(Elem* pValorA,Elem* pValorB)
{
  Elem* pPvtA    = NULL; /* elemento atual no valor A */
  Elem* pPvtB    = NULL; /* elemento atual no valor B */ 
  Elem* pSoma    = NULL;
  Elem* pSomaAtu = NULL;
  Elem* pSomaAux = NULL;
  int   nCarry   = 0;
  int   nSoma    = 0;

  /* inicializacao dos pivots */
  pPvtA = pValorA;
  pPvtB = pValorB;
  /* se ambos forem validos... */
  if(pPvtA!=NULL && pPvtB!=NULL)
  {
    /* faz uma primeira pra inicializar os ponteiros */
    nSoma  = (pPvtA->digito + pPvtB->digito);
    nCarry = nSoma/BASE_NUM;
    nSoma  = nSoma%BASE_NUM;
    /* inicializa ponteiros da lista de soma */
    pSoma = pSomaAtu = criaElem(nSoma);
    pPvtA = pPvtA->prox;
    pPvtB = pPvtB->prox;
    /* enquanto AMBOS forem validos */
    while(pPvtA!=NULL && pPvtB!=NULL)
    {
      /* soma digitos + vai um  */
      nSoma  = nCarry + (pPvtA->digito + pPvtB->digito);
      nCarry = nSoma/BASE_NUM;
      nSoma  = nSoma%BASE_NUM;
      pSomaAtu->prox = criaElem(nSoma);
      pSomaAtu = pSomaAtu->prox;
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
    nSoma  = nSoma%BASE_NUM;
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
    pPvtA = pPvtA->prox;
    while(pPvtA!=NULL)
    {
      /* soma carry ao digito (semelhante ao loop acima) */
      nSoma  = (nCarry + pPvtA->digito);
      nCarry = nSoma/BASE_NUM;
      nSoma  = nSoma%BASE_NUM;
      /* novo proximo item sera um item novo (vixe!) */
      pSomaAtu->prox = criaElem(nSoma);
      pSomaAtu = pSomaAtu->prox;
      pPvtA = pPvtA->prox;
    }
  }
  else if(pPvtB!=NULL)
  {
    /* entao, ou sobrou em B, ou se somente B eh valido */
    /* soma digito do valor B + vai um */
    nSoma  = (nCarry + pPvtB->digito);
    nCarry = nSoma/BASE_NUM;
    nSoma  = nSoma%BASE_NUM;
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
    pPvtB = pPvtB->prox;
    while(pPvtB!=NULL)
    {
      /* soma carry ao digito (semelhante ao loop acima) */
      nSoma  = (nCarry + pPvtB->digito);
      nCarry = nSoma/BASE_NUM;
      nSoma  = nSoma%BASE_NUM;
      /* novo proximo item sera um item novo (vixe!) */
      pSomaAtu->prox = criaElem(nSoma);
      pSomaAtu = pSomaAtu->prox;
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
----------------------------------------------------------*/
Elem* multiplicaLista(Elem* pValorA,Elem* pValorB)
{
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

  /* aborta se fator B for invalido  */
  if(pValorB==NULL) return(NULL);
  /* percorre fator A */
  pPvtA = pValorA;
  while(pPvtA!=NULL)
  {
    /* se o digito atual do falot A for zero, 
    junta mais um zero na lista de zero e vai para o proximo digito de A */
    if(pPvtA->digito==0)
    {
      if(pLastZero!=NULL) pLastZero->prox = NULL;
      /* adiciona um zero na lista de padding */
      /* se a lista de zeros estiver vazia, ela eh inicializada */
      pLastZero = criaElem(0);
      pListaZero = apendElem(pListaZero,pLastZero);
      pPvtA = pPvtA->prox;
      continue; /* while(pPvtA!=NULL) */
    }
    /* inicializa valores de controle */
    pPvtB = pValorB;
    nCarry = 0;
    /* produto do primeiro digito eh externo, para inicializar a lista */
    nProd  = (pPvtA->digito * pPvtB->digito);
    nCarry = nProd/BASE_NUM;
    nProd  = nProd%BASE_NUM;
    pPvtB = pPvtB->prox;

    pSubProdIni = pSubProdAtu = criaElem(nProd);
    
    /* junta lista de zeros, se houver, com inicio do subproduto */
    /* se a lista de zeros estiver vazia, o subproduto para soma sera o proprio valor de inicio,
    se nao, sera a lista composta pela lista de zeros e o inicio dos produtos*/
    pSubProd = apendElem(pListaZero,pSubProdIni);

    /* percorre fator B restantes */
    while(pPvtB!=NULL)
    {
      /* realiza demais produtos dos digitos */
      nProd  = nCarry + (pPvtA->digito * pPvtB->digito);
      nCarry = nProd/BASE_NUM;
      nProd  = nProd%BASE_NUM;
      /* armazena um novo elemento no final do subproduto */
      pSubProdAtu->prox = criaElem(nProd);
      pSubProdAtu = pSubProdAtu->prox;
      pPvtB = pPvtB->prox;
    }
    
    /* sobrou um, vai mais um! */
    if(nCarry>0)
    {
      pSubProdAtu->prox = criaElem(nCarry);
      pSubProdAtu = pSubProdAtu->prox;
    }
    /* acumula subproduto COM ZEROS A DIREITA no produto final */
    pProdFinal = somaLista(pProdFinal,pSubProd);
    /* libera lista utilizada pelo subproduto SEM ZEROS A DIREITA */
    freeLista(pSubProdIni); 
    if(pLastZero!=NULL) pLastZero->prox = NULL;
    /* adiciona um zero na lista de padding */
    /* se a lista de zeros estiver vazia, ela eh inicializada */
    pLastZero = criaElem(0);
    pListaZero = apendElem(pListaZero,pLastZero);
    pPvtA = pPvtA->prox;
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
  Elem* pValorA  = NULL;        /* lista ligada contendo o valor 'invertido' do arquivo A */
  Elem* pValorB  = NULL;        /* lista ligada contendo o valor 'invertido' do arquivo A */
  Elem* pResult  = NULL;        /* lista ligada contendo o valor final da operacao */
  char  szArquivoA[MAX_PATH+1]; /* path para o arquivo A */
  char  szArquivoB[MAX_PATH+1]; /* path para o arquivo B */
  char  szArqSaida[MAX_PATH+1]; /* path para o arquivo de saida */
  int   nOper    =0;            /* numero da opcao desejada */
  int   bOk      =1;            /* flag de retorno */

#ifndef __STDC__
  printf("Este programa nao foi gerado com padronizacao ANSI C!\n");
#endif

  /* Verificacao simples, a chamada deve ter 13 argumentos; */
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
        default : bOk=0;                                  break;
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
  /* le um arq entrada os numeros, convertidos em lista ligada, em ordem reversa */
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

  /* le um arq entrada os numeros, convertidos em lista ligada, em ordem reversa */
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
  /* executa a funcao atribuida */
  pResult = funcOper(pValorA,pValorB);
  /* libera listas de entrada */
  freeLista(pValorB);
  freeLista(pValorA);
  if(pResult==NULL)
  {
    printf("Erro processando as listas\n",szArqSaida);
    return(13);
  }
  /* reinverte a lista resultante */
  pResult = inverteLista(pResult);

#ifdef DEBUG_PROG
  printf("%s:\n",szArqSaida);
  printLista(pResult);
#endif /* DEBUG_PROG */

  /* grava lista resultante invertida em arquivo */
  bOk = writeLista2File(szArqSaida,pResult);
  freeLista(pResult);
  if(!bOk)
  {
    printf("Erro gravando o arquivo de destino '%s'.\n",szArqSaida);
    return(14);
  }
  return(0);
}

