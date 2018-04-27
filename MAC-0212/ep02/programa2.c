/***************************************************************************************************
 * Thiago Pinheiro de Macedo - N USP: 5124272
 * MAC 0212 - Laboratório de Computação
 * IME USP - BMAC - Noturno - Primeiro Semestre de 2006
 * Exercício Programa 2 - Prof. Marcilio - Entregar - 28Mai06
 **************************************************************************************************/
/***************************************************************************************************
 * Includes da aplicacao
 **************************************************************************************************/
#include "mac0212ep02.h"

/***************************************************************************************************
 * Abre o arquivo szNomeArq com parametros szParam (veja fopen()),
 * com siclo de 20 tentativas e 5 ms de delay entre elas;
 **************************************************************************************************/
FILE* AbreArquivo(char* szNomeArq, char* szParam)
{
	FILE*  fpArq  = NULL;
	int    nTenta = 0;

	while(nTenta < 20)
	{
		if((fpArq = fopen(szNomeArq, szParam)) != NULL) 
		{
			fseek(fpArq, 0, SEEK_SET);
			return(fpArq);
		}
		/* sleep(5); nao eh ANSI! */
		nTenta++;
	}
	printf("Nao foi possivel abrir o arquivo especificado: '%s';\n", szNomeArq);
	return(NULL);
}
/***************************************************************************************************
 * Abre os arquivos utilizados pelo programa;
 **************************************************************************************************/
int AbreArquivos(ARQUIVOS* pArquivos)
{
	/** 
	 * -> Abre arquivo principal de clientes para leitura; 
	 * -> Aborta se nao existir; 
	 **/
	if((pArquivos->fpArqCli = AbreArquivo(ARQ_CLI_NOME, "rb")) == NULL)
	{
		return(FALSE);
	}
	/** 
	 * -> Abre arquivo de inclusoes de clientes para leitura e escrita; 
	 * -> Aborta se nao existir; 
	 **/
	if((pArquivos->fpArqInc = AbreArquivo(ARQ_INC_NOME, "r+b")) == NULL)
	{
		fclose(pArquivos->fpArqCli);
		return(FALSE);
	}
	/** 
	 * -> Cria um arquivo para realizar o merge dos arquivos anteriores; 
	 * -> Se jah existir, o conteudo eh destruido;
	 **/
	if((pArquivos->fpArqMer = AbreArquivo(ARQ_MER_NOME, "w+b")) == NULL)
	{
		fclose(pArquivos->fpArqCli);
		fclose(pArquivos->fpArqInc);
		return(FALSE);
	}
	return(TRUE);
}
/***************************************************************************************************
 * Fecha os arquivos utilizados pelo programa;
 **************************************************************************************************/
void FechaArquivos(ARQUIVOS* pArquivos)
{
	if(pArquivos->fpArqCli != NULL)
	{
		fclose(pArquivos->fpArqCli);
		pArquivos->fpArqCli = NULL;
	}
	if(pArquivos->fpArqInc != NULL)
	{
		fclose(pArquivos->fpArqInc);
		pArquivos->fpArqInc = NULL;
	}
	if(pArquivos->fpArqMer != NULL)
	{
		fclose(pArquivos->fpArqMer);
		pArquivos->fpArqMer = NULL;
	}
}
/***************************************************************************************************
 * Obtem o tamanho do arquivo;
 **************************************************************************************************/
int MyGetFileSize(FILE* fpArq)
{
	int nSize;
	
	fseek(fpArq, 0, SEEK_END);
	nSize = ftell(fpArq);
	fseek(fpArq, 0, SEEK_SET);
	return(nSize);
}
/***************************************************************************************************
 * Metodo de Particionamento utilizando em QuickSort (abaixo)
 **************************************************************************************************/
int particiona(int pContas[], int pIdx[], int nInicio, int nFim)
{
  register int  i, j, nAux;
           int  nPivot[2];

  nPivot[0] = pContas[nFim];
  nPivot[1] = pIdx[nFim];
  i = (nInicio - 1);
  for(j = nInicio; j < nFim; j++)
  {
    if(pContas[j] <= nPivot[0])
    {
      i++;
      /* swap no array de contas; */
      nAux = pContas[j];
      pContas[j] = pContas[i];
      pContas[i] = nAux;
      /* swap no array de indices; */
      nAux = pIdx[j];
      pIdx[j] = pIdx[i];
      pIdx[i] = nAux;
    }
  }
  i++;
  /* atualiza pivot no array de contas; */
  pContas[nFim] = pContas[i];
  pContas[i]    = nPivot[0];
  /* atualiza pivot no array de indices; */
  pIdx[nFim] = pIdx[i];
  pIdx[i]    = nPivot[1];
  return(i);
}
/***************************************************************************************************
 * Metodo QuickSort customizado para ordenacao conjunta;
 **************************************************************************************************/
void QuickSort(int pContas[], int pIdx[], int nInicio, int nFim)
{
  int nPivot;

  /* base da recursao eh o inicio do vetor ser igual ao final */
  if(nInicio < nFim)
  {
    /* passo da recursao consiste no particionamento do vetor, ordenando um pivot */
    nPivot = particiona(pContas, pIdx, nInicio, nFim);
    QuickSort(pContas, pIdx, nInicio, nPivot - 1);
    QuickSort(pContas, pIdx, nPivot + 1, nFim);
  }
}
/***************************************************************************************************
 * Ordena um arquivo com registros do tipo CLIENTE;
 **************************************************************************************************/
int OrdenaArquivo(FILE* fpArq)
{
	FILE*    fpArqNew;
	CLIENTE  sCliente;
	int*     pListaIdx;
	int*     pListaContas;
	int      nNumReg;
	int      nPos;
	int      bOk = TRUE;
	
	nNumReg = MyGetFileSize(fpArq) / sizeof(CLIENTE);
	if(!nNumReg)
	{
		/* Nada a ser ordenado */
		return(FALSE);
	}
	else if(nNumReg == 1)
	{
		/* Um registro sempre estar ordenado! */
		return(TRUE);
	}
	/* Aloca listas de contas e indices; */
	pListaIdx = (int*) malloc(nNumReg * sizeof(CLIENTE));
	if(pListaIdx == NULL)
	{
		return(FALSE);
	}
	pListaContas = (int*) malloc(nNumReg * sizeof(CLIENTE));
	if(pListaContas == NULL)
	{
		free(pListaIdx);
		return(FALSE);
	}
	/* Preenche lista em memoria com contas do arquivo e suas posicoes no mesmo; */
	nPos = 0;
	fseek(fpArq, 0, SEEK_SET);
	while(fread(&sCliente, 1, sizeof(CLIENTE), fpArq) == sizeof(CLIENTE))
	{
		pListaContas[nPos] = sCliente.nConta;
		pListaIdx[nPos] = nPos;
		nPos++;
	}
	/* Ordena lista de contas, em conjunto com a lista de indices; */
	QuickSort(pListaContas, pListaIdx, 0, (nNumReg - 1));
	fpArqNew = AbreArquivo(ARQ_TMP_NOME,"w+b");
	/* Percorre a lista de contas ordenadas e grava novo arquivo; */
	fseek(fpArqNew, 0, SEEK_SET);
	for(nPos = 0; nPos < nNumReg; nPos++)
	{
		fseek(fpArq, (pListaIdx[nPos] * sizeof(CLIENTE)) , SEEK_SET);
		if(fread(&sCliente, 1, sizeof(CLIENTE), fpArq) != sizeof(CLIENTE))
		{
			printf("-> Erro lendo registro no arquivo de inclusoes!\n");
			bOk = FALSE;
			break;
		}
		if(fwrite(&sCliente, 1, sizeof(CLIENTE), fpArqNew) != sizeof(CLIENTE))
		{
			printf("-> Erro gravando registro no arquivo ordenado!\n");
			bOk = FALSE;
			break;
		}
	}
	if(bOk)
	{
		/* Copia registros para o arquivo original; */
		fseek(fpArq, 0, SEEK_SET);
		fseek(fpArqNew, 0, SEEK_SET);
		while(fread(&sCliente, 1, sizeof(CLIENTE), fpArqNew) == sizeof(CLIENTE))
		{
			if(fwrite(&sCliente, 1, sizeof(CLIENTE), fpArq) != sizeof(CLIENTE))
			{
				printf("-> Erro copiando registros para o arquivo de inclusoes!\n");
				bOk = FALSE;
				break;
			}
		}
	}
	fclose(fpArqNew);
	/* remove o arquivo temporario; */
	remove(ARQ_TMP_NOME);
	/* Libera listas alocadas; */
	free(pListaContas);
	free(pListaIdx);
	return(bOk);
}
/***************************************************************************************************
 * Realiza o Swap entre duas structs cliente;
 * TODO : VERIFICAR SE O REGISTRO LIDO EH VALIDO!!!
 **************************************************************************************************/
int MergeArquivos(ARQUIVOS* pArquivos)
{
	CLIENTE  sCliMain, sCliInc;
	int      nNumRegCli, nNumRegInc;
	int      nPosCli, nPosInc;
	int      bOk = TRUE;

	/* Reseta posicoes; */
	nPosCli = nPosInc = 0;
	
	/* Obtem os tamanhos dos arquivos; */
	nNumRegCli = (MyGetFileSize(pArquivos->fpArqCli) / sizeof(CLIENTE));
	nNumRegInc = (MyGetFileSize(pArquivos->fpArqInc) / sizeof(CLIENTE));

	/* Posiciona no inicio do novo arquivo; */
	fseek(pArquivos->fpArqMer, 0, SEEK_SET);
	
	/* Le um registro do arquivo de clientes; */
	do {
		bOk = (fread(&sCliMain, 1, sizeof(CLIENTE), pArquivos->fpArqCli) == sizeof(CLIENTE));
		nPosCli++;
	} while(bOk && (sCliMain.cValido != CLIENTE_VALIDO));
	/* invalida se nao houve um registro valido; */
	if(!bOk)  memset(&sCliMain, 0, sizeof(CLIENTE));

	/* Le um registro do arquivo de inclusoes; */
	do {
		bOk = (fread(&sCliInc, 1, sizeof(CLIENTE), pArquivos->fpArqInc) == sizeof(CLIENTE));
		nPosInc++;
	} while(bOk && (sCliInc.cValido != CLIENTE_VALIDO));
	/* invalida se nao houve um registro valido; */
	if(!bOk)  memset(&sCliInc, 0, sizeof(CLIENTE));

	/* enquanto ouverem ocorrencias em ambos; */
	while((nPosCli < nNumRegCli) && (nPosInc < nNumRegInc))
	{
		if(sCliMain.nConta < sCliInc.nConta)
		{
			fwrite(&sCliMain, 1, sizeof(CLIENTE), pArquivos->fpArqMer);
			/* Le um novo registro do arquivo de clientes; */
			do {
				bOk = (fread(&sCliMain, 1, sizeof(CLIENTE), pArquivos->fpArqCli) == sizeof(CLIENTE));
				nPosCli++;
			} while(bOk && (sCliMain.cValido != CLIENTE_VALIDO));
			/* invalida se nao houve um registro valido; */
			if(!bOk)  memset(&sCliMain, 0, sizeof(CLIENTE));
		}
		else
		{
			fwrite(&sCliInc, 1, sizeof(CLIENTE), pArquivos->fpArqMer);
			/* Le novo um registro do arquivo de inclusoes; */
			do {
				bOk = (fread(&sCliInc, 1, sizeof(CLIENTE), pArquivos->fpArqInc) == sizeof(CLIENTE));
				nPosInc++;
			} while(bOk && (sCliInc.cValido != CLIENTE_VALIDO));
			/* invalida se nao houve um registro valido; */
			if(!bOk)  memset(&sCliInc, 0, sizeof(CLIENTE));
			
		}
	}
	/* verifica se sobrou um registro do arquivo principal; */
	if(sCliMain.nConta > 0)
	{
		fwrite(&sCliMain, 1, sizeof(CLIENTE), pArquivos->fpArqMer);
	}
	/* verifica se sobrou um registro do arquivo de inclusoes; */
	if(sCliInc.nConta > 0)
	{
		fwrite(&sCliInc, 1, sizeof(CLIENTE), pArquivos->fpArqMer);
	}
	/* Grava restante do arquivo principal; */
	while(fread(&sCliMain, 1, sizeof(CLIENTE), pArquivos->fpArqCli) == sizeof(CLIENTE))
	{
		fwrite(&sCliMain, 1, sizeof(CLIENTE), pArquivos->fpArqMer);
	}
	/* Grava restante do arquivo de inclusoes; */
	while(fread(&sCliInc, 1, sizeof(CLIENTE), pArquivos->fpArqInc) == sizeof(CLIENTE))
	{
		fwrite(&sCliInc, 1, sizeof(CLIENTE), pArquivos->fpArqMer);
	}
	/* Retorna TRUE se gravou alguma coisa no arquivo de Merge; FALSE caso contrario; */
	return(ftell(pArquivos->fpArqMer) > 0);
}
/***************************************************************************************************
 * Verifica se um arquivo de clientes esta ordenado;
 **************************************************************************************************/
int IsArquivoOrdenado(FILE* fpArq)
{
	CLIENTE  sCliente;
	char     szNome[31];
	int      nConta = 0;

	fseek(fpArq, 0, SEEK_SET);
	while(fread(&sCliente, 1, sizeof(CLIENTE), fpArq) == sizeof(CLIENTE))
	{
		/* copia nome para buffer local; forca ultima posicao = '\0'; */
		strncpy(szNome, sCliente.szNome, 30);  szNome[30] = '\0';
		/* Exibe dados em stdout; */
		printf("\t  Conta : %d\n"   , sCliente.nConta);
		printf("\t   Nome : %s\n"   , szNome);
		printf("\t  Saldo : %.2lf\n", sCliente.fSaldo);
		printf("\t Valido ? %s\n"   , (sCliente.cValido==CLIENTE_VALIDO ? "SIM" : "NAO"));
		printf("\n");
		/* Verifica se esta em ordem; */
		if(sCliente.nConta < nConta) return(FALSE);
		nConta = sCliente.nConta;
	}
	return(TRUE);
}
/***************************************************************************************************
 * Funcao Principal; Entry-point do programa;
 **************************************************************************************************/
int main(void)
{
	ARQUIVOS  sArquivos;
	int       bOk = FALSE;

	/* abre arquivos utilizados pela aplicacao; */
	if(!AbreArquivos(&sArquivos))
	{
		return(1);
	}
	printf("Programa 02 - Consolidaca de Arquivos de Clientes;\n");
	printf("-> Ordenando arquivo de inclusoes;\n");
	if(OrdenaArquivo(sArquivos.fpArqInc))
	{
		printf("-> Consolidando arquivo de inclusoes ao arquivo principal;\n");
		if(MergeArquivos(&sArquivos))
		{
			printf("-> Verificando arquivo consolidado;\n");
			if(IsArquivoOrdenado(sArquivos.fpArqMer))
			{
				printf("-> Verificacao concluida com sucesso!\n");
				bOk = TRUE;
			}
			else
			{
				printf("-> Arquivo nao esta ordenado!!!\n");
			}
		}
	}
	/* fecha arquivos utilizados pela aplicacao; */
	FechaArquivos(&sArquivos);
	printf("\nFinalizado!\n");
    _getch();
	return(!bOk);
}
