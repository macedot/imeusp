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
 * API para ler um caracter de stdin;
 **************************************************************************************************/
int myGetChar()
{
  int  ch = 0;

  fflush(stdin);
#ifdef EP_WINDOWS
	/* versao WINDOWS */
	ch = tolower(_getch());
#else
	/* versao POSIX */
	ch = tolower(getchar());
#endif
	return(ch);
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
 * Exibe uma mensagem ao usuario e aguarda o input das teclas 's' ou 'n',
 * retornando TRUE ou FALSE, respectivamente;
 **************************************************************************************************/
int MsgConfirma(char* szMsg)
{
  int  ch = 0;

  /* exibe a mensagem para o usuario */
  printf("%s [s|n] :",szMsg);
  do {
    ch = myGetChar();
  } while((ch!='s') && (ch!='n'));
  printf(" (%c)\n", ch);
  /* retorna TRUE se (ch=='s'); FALSE, caso contrario; */
  return((ch=='s') ? TRUE : FALSE);
}
/***************************************************************************************************
 * Cria menu de opcoes do programa :
 * (d) débito, (c) crédito , (i) inclusão, (e) exclusão;
 **************************************************************************************************/
int MenuOpcao(void)
{
	int  ch = 0;
	int  i  = 0;
	
	/* exibe a mensagem para o usuario */
	for(i = 0; i < 50; i++) printf("*");
	printf("\nMenu de Opcoes:\n");
	printf("\t(c) Credito em Conta;\n");
	printf("\t(d) Debito em Conta;\n");
	printf("\t(i) Inclusao de Cliente;\n");
	printf("\t(e) Exclusao de Cliente;\n");
	printf("\t(f) Finalizar o programa;\n");
	printf("Digite sua opcao: ");
	do {
	    ch = myGetChar();
	} while(!((ch=='c') | (ch=='d') | (ch=='i') | (ch=='e') | (ch=='f')));
	printf("(%c)\n", ch);
	for(i = 0; i < 50; i++) printf("*");
	printf("\n");
	/* returna a opcao selecionada */
	return(ch);
}
/***************************************************************************************************
 * Le uma string apartir de stdin;
 **************************************************************************************************/
int GetStr(char* szStr)
{
	char*  pAux = NULL;

	do {
		/* "limpa" o buffer de entrada */
		fflush(stdin);
		/* le uma string */
		gets(szStr);
		/* workaround para linux: fflush "nao funciona" nos linux testados; */
		pAux = szStr;
		/* percorre ate o fim */
		while(*pAux!='\0')
		{
			/* se for um token, pula para o proximo; */
			/* se for outro caracter qualquer, interrompe, pois o buffer eh valido; */
			if((*pAux!='\r') && (*pAux!='\n'))  break;
			pAux++;
		}
		/* repete enquanto o buffer nao for algo valido; */
	} while(*pAux=='\0');
	return(TRUE);
}
/***************************************************************************************************
 * Abre o arquivo szNomeArq com parametros szParam (veja fopen()),
 * com siclo de 20 tentativas e 5 ms de delay entre elas;
 **************************************************************************************************/
FILE* AbreArquivo(char* szNomeArq, char* szParam)
{
	FILE*  fpArq  = NULL;
	int    nTenta = 0;

	while(nTenta++ < 20)
	{
		if((fpArq = fopen(szNomeArq, szParam)) != NULL) 
		{
			fseek(fpArq, 0, SEEK_SET);
			return(fpArq);
		}
		/* sleep(5); nao eh ANSI! */
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
	 * -> Abre arquivo principal de clientes para leitura e escrita; 
	 * -> Aborta se nao existir; 
	 * -> Se jah existir, o conteudo eh mantido;
	 **/
	if((pArquivos->fpArqCli = AbreArquivo(ARQ_CLI_NOME, "r+b")) == NULL)
	{
		return(FALSE);
	}
	/** 
	 * -> Abre arquivo de inclusoes de clientes para leitura e escrita; 
	 * -> Cria o arquivo, se nao existir; 
	 * -> Se jah existir, o conteudo eh destruido!;
	 **/
	if((pArquivos->fpArqInc = AbreArquivo(ARQ_INC_NOME, "r+b")) == NULL)
	{
		if((pArquivos->fpArqInc = AbreArquivo(ARQ_INC_NOME, "w+b")) == NULL)
		{
			fclose(pArquivos->fpArqCli);
			return(FALSE);
		}
	}
	return(TRUE);
}
/***************************************************************************************************
 * Fecha os arquivos utilizados pelo programa;
 **************************************************************************************************/
void FechaArquivos(ARQUIVOS* pArquivos)
{
	if(pArquivos->fpArqCli!=NULL)
	{
		fclose(pArquivos->fpArqCli);
		pArquivos->fpArqCli = NULL;
	}
	if(pArquivos->fpArqInc!=NULL)
	{
		fclose(pArquivos->fpArqInc);
		pArquivos->fpArqInc = NULL;
	}
}
/***************************************************************************************************
 * Le um registro utlizando os parametros de pFindCli;
 **************************************************************************************************/
int GetCliente(FINDCLI* pFindCli)
{
	fseek(pFindCli->fpArq, (pFindCli->nPos * sizeof(CLIENTE)), SEEK_SET);
	if(fread(&(pFindCli->sCliente), 1, sizeof(CLIENTE), pFindCli->fpArq) == sizeof(CLIENTE))
	{
		return(TRUE);
	}
	return(FALSE);
}
/***************************************************************************************************
 * Grava um registro utlizando os parametros de pFindCli;
 **************************************************************************************************/
int SetCliente(FINDCLI* pFindCli)
{
	if(pFindCli->nPos < 0)
	{
		/* se for informada uma posicao de valor negativo, contatena registro no final do arquivo; */
		fseek(pFindCli->fpArq, 0, SEEK_END);
	}
	else
	{
		/* caso contrario, posiciona para leitura no registro correspondente; */
		fseek(pFindCli->fpArq, (pFindCli->nPos * sizeof(CLIENTE)), SEEK_SET);
	}
	if(fwrite(&(pFindCli->sCliente), 1, sizeof(CLIENTE), pFindCli->fpArq) == sizeof(CLIENTE))
	{
		/* Forca a gravacao dos dados em disco; */
		fflush(pFindCli->fpArq);
		return(TRUE);
	}
	return(FALSE);
}
/***************************************************************************************************
 * Exibe na tela os dados de um cliente
 **************************************************************************************************/
void DumpCliente(FINDCLI* pFindCli)
{
	char  szNome[31];
	int   i = 0;
	
	/* copia nome para buffer local; forca ultima posicao = '\0'; */
	strncpy(szNome, pFindCli->sCliente.szNome, 30);  szNome[30] = '\0';
	for(i = 0; i < 50; i++) printf("*");
	printf("\n");
	/* Exibe dados em stdout; */
	printf("\t  Index : %06d\n" , pFindCli->nPos);
	printf("\t Offset : 0x%04X\n" , pFindCli->nPos * sizeof(CLIENTE));
	printf("\t  Conta : %d\n"   , pFindCli->sCliente.nConta);
	printf("\t   Nome : %s\n"   , szNome);
	printf("\t  Saldo : %.2lf\n", pFindCli->sCliente.fSaldo);
	printf("\t Valido ? %s\n"   , (pFindCli->sCliente.cValido==CLIENTE_VALIDO ? "SIM" : "NAO"));
	for(i = 0; i < 50; i++) printf("*");
	printf("\n");
}
/***************************************************************************************************
 * Localiza um cliente atraves de leitura sequencial (arquivo nao ordenado);
 **************************************************************************************************/
int LocalizaClienteBuscaSeq(FINDCLI* pFindCli)
{
	CLIENTE  sCliente;
	int      nPos = 0;

	/* Obtem qual o indice do ultimo registro valido no arquivo informado; */
	fseek(pFindCli->fpArq, 0, SEEK_SET);
	while(fread(&sCliente, 1, sizeof(CLIENTE), pFindCli->fpArq) == sizeof(CLIENTE))
	{
		/* Verifica se a conta eh esta */
		if(sCliente.nConta == pFindCli->sCliente.nConta)
		{
			/* Copia struct lida para o buffer e guarda posicao no arquivo; */
			memcpy(&(pFindCli->sCliente), &sCliente, sizeof(CLIENTE));
			pFindCli->nPos = nPos;
			return(TRUE);
		}
		/* 
		NOTA: -> NAO abortamos a busca em arquivos sequenciais, pois os mesmo, sendo de inclusao,
		      podem conte N-registros com a MESMA conta; 
		      Ex: Inclusao, exclusao, (...), Inclusao;
		*/
		nPos++;
	}
	return(FALSE);
}
/***************************************************************************************************
 * Localiza um cliente atraveis de busca binaria (arquivo ordenado);
 **************************************************************************************************/
int LocalizaClienteBuscaBin(FINDCLI* pFindCli)
{
	FINDCLI  sFindCli;
	int      low  = 0;
	int      mid  = 0;
	int      high = 0;

	memcpy(&sFindCli, pFindCli, sizeof(FINDCLI));
	/* Obtem qual o indice do ultimo registro valido no arquivo informado; */
	high = (MyGetFileSize(sFindCli.fpArq) / sizeof(CLIENTE));
	/* Percorre enquantro nao ouver troca de pivot */
	while(low <= high)
	{
		mid = (low + high) / 2;
		sFindCli.nPos = mid;
		if(!GetCliente(&sFindCli))  break;
		if(sFindCli.sCliente.nConta > pFindCli->sCliente.nConta)
		{
			/* Conta localizada eh maior que a desejada; -> espaço de busca eh o inferior; */
			high = mid - 1;
		}
		else if(sFindCli.sCliente.nConta < pFindCli->sCliente.nConta)
		{
			/* Conta localizada eh menor que a desejada; -> espaço de busca eh o superior; */
			low = mid + 1;
		}
		else 
		{
			/* Achou -> Copia struct lida para o buffer e guarda posicao no arquivo; */
			memcpy(pFindCli, &sFindCli, sizeof(FINDCLI));
			return(TRUE);
		}
	}
	return(FALSE);
}
/***************************************************************************************************
 * Localiza um cliente apartir do numero da conta;
 **************************************************************************************************/
int LocalizaCliente(ARQUIVOS* pArquivos, FINDCLI* pFindCli)
{
	/* Busca primeiro no arquivo principal, via busca binaria... */
	pFindCli->fpArq = pArquivos->fpArqCli;
	if(!LocalizaClienteBuscaBin(pFindCli))
	{
		/* ... se nao achou, busca no arquivo de inclusoes, via busca sequencial... */
		pFindCli->fpArq = pArquivos->fpArqInc;
		if(!LocalizaClienteBuscaSeq(pFindCli))
		{
			/* ... caramba, nao achou mesmo, entao retorna FALSE! */
			pFindCli->fpArq = NULL;
			return(FALSE);
		}
	}
	DumpCliente(pFindCli);
	return(TRUE);
}
/***************************************************************************************************
 * Realiza uma operacao de credito em conta;
 **************************************************************************************************/
int CreditaCliente(ARQUIVOS* pArquivos, FINDCLI*  pFindCli)
{
	char  szBuffer[256];

	/* exibe a mensagem para o usuario */
	printf("-> Credito em Conta;\n");
	/* Obtem a conta para localiza no arquivo; */
	printf("Informe a conta para credito: ");
	GetStr(szBuffer);
	pFindCli->sCliente.nConta = (int) atol(szBuffer);
	/* Localiza o cliente nos arquivos ; */
	if(!LocalizaCliente(pArquivos, pFindCli))
	{
		printf("-> A conta '%d' nao pertence a nenhum cliente valido!\n", pFindCli->sCliente.nConta);
		return(FALSE);
	}
    /* Verifica se a conta encontrada eh valida */
    if(pFindCli->sCliente.cValido != CLIENTE_VALIDO)
    {
		printf("-> A conta '%d' foi excluida e nao pode ser creditada!\n", pFindCli->sCliente.nConta);
		return(FALSE);
    }
	/* Obtem o valor para debito em conta; */
	printf("Informe o valor a ser creditado : ");
	GetStr(szBuffer);
	/* deduz do saldo o valor informado */
	pFindCli->sCliente.fSaldo += (double) atof(szBuffer);
	/* Grava cliente alterado no respectivo arquivo; */
	return(SetCliente(pFindCli));
}
/***************************************************************************************************
 * Realiza uma operacao de debito em conta;
 **************************************************************************************************/
int DebitaCliente(ARQUIVOS* pArquivos, FINDCLI*  pFindCli)
{
	char  szBuffer[256];

	/* exibe a mensagem para o usuario */
	printf("-> Debito em Conta;\n");
	/* Obtem a conta para localiza no arquivo; */
	printf("Informe a conta para debito: ");
	GetStr(szBuffer);
	pFindCli->sCliente.nConta = (int) atol(szBuffer);
	/* Localiza o cliente nos arquivos ; */
	if(!LocalizaCliente(pArquivos, pFindCli))
	{
		printf("-> A conta '%d' nao pertence a nenhum cliente valido!\n", pFindCli->sCliente.nConta);
		return(FALSE);
	}
    /* Verifica se a conta encontrada eh valida */
    if(pFindCli->sCliente.cValido != CLIENTE_VALIDO)
    {
		printf("-> A conta '%d' foi excluida e nao pode ser debitada!\n", pFindCli->sCliente.nConta);
		return(FALSE);
    }
	/* Obtem o valor para debito em conta; */
	printf("Informe o valor a ser debidato : ");
	GetStr(szBuffer);
	/* deduz do saldo o valor informado */
	pFindCli->sCliente.fSaldo -= (double) atof(szBuffer);
	/* Grava cliente alterado no respectivo arquivo; */
	return(SetCliente(pFindCli));
}
/***************************************************************************************************
 * Realiza uma operacao de inclusao de cliente;
 **************************************************************************************************/
int IncluiCliente(ARQUIVOS* pArquivos, FINDCLI*  pFindCli)
{
	char  szBuffer[256];

	/* exibe a mensagem para o usuario */
	printf("-> Inclusao de Cliente;\n");
	/* Obtem a conta para localiza no arquivo; */
	do {
		printf("\tConta : ");
		GetStr(szBuffer);
		pFindCli->sCliente.nConta = (int) atol(szBuffer);
	} while(pFindCli->sCliente.nConta <= 0);
	/* Localiza o cliente nos arquivos ; */
	if(LocalizaCliente(pArquivos, pFindCli))
	{
		printf("-> Conta '%d' ja existe; Operacao cancelada;\n", pFindCli->sCliente.nConta);
		return(FALSE);
	}
	/* obtem dados restantes para gravacao */
	printf("\t Nome : ");
	GetStr(szBuffer);
	strncpy(pFindCli->sCliente.szNome, szBuffer, 29);
	pFindCli->sCliente.szNome[29] = '\0';
	printf("\tSaldo : ");
	GetStr(szBuffer);
	pFindCli->sCliente.fSaldo = (double) atof(szBuffer);
	/* Marca cliente como valido; */
	pFindCli->sCliente.cValido = CLIENTE_VALIDO;
	pFindCli->fpArq = pArquivos->fpArqInc;
	pFindCli->nPos = -1; /* append */
	/* Grava cliente alterado no respectivo arquivo; */
	return(SetCliente(pFindCli));
}
/***************************************************************************************************
 * Realiza uma operacao de exclusao de cliente;
 **************************************************************************************************/
int ExcluiCliente(ARQUIVOS* pArquivos, FINDCLI*  pFindCli)
{
	char  szBuffer[256];

	/* exibe a mensagem para o usuario */
	printf("-> Exclusao de Cliente;\n");
	/* Obtem a conta para localiza no arquivo; */
	printf("Informe a conta do cliente apra exclusao: ");
	GetStr(szBuffer);
	pFindCli->sCliente.nConta = (int) atol(szBuffer);
	/* Localiza o cliente no arquivo e verifica se o mesmo eh valido; */
	if(!LocalizaCliente(pArquivos, pFindCli))
	{
		printf("-> A conta '%d' nao pertence a nenhum cliente valido!\n", pFindCli->sCliente.nConta);
		return(FALSE);
	}
    /* Verifica se a conta encontrada eh valida */
    if(pFindCli->sCliente.cValido != CLIENTE_VALIDO)
    {
		printf("-> A conta '%d' jah foi excluida anteriormente!\n", pFindCli->sCliente.nConta);
		return(FALSE);
    }
	if(!MsgConfirma("Confirma a exclusao deste cliente ?"))
	{
		return(FALSE);
	}
	/* altera status do cliente */
	pFindCli->sCliente.cValido = 'n';
	/* Grava cliente alterado no respectivo arquivo; */
	return(SetCliente(pFindCli));
}
/***************************************************************************************************
 * Funcao Principal; Entry-point do programa;
 **************************************************************************************************/
int main(void)
{
	ARQUIVOS  sArquivos;
	FINDCLI   sFindCli;
	int       nOpc = 0;

	/* abre arquivos utilizados pela aplicacao; */
	if(!AbreArquivos(&sArquivos))
	{
        return(1);
	}
	printf("Programa 01 - Manutencao de Registros;\n");
	while((nOpc = MenuOpcao()) != 'f')
	{
		memset(&sFindCli, 0, sizeof(FINDCLI));
		switch(nOpc)
		{
			/* c: entre com conta e valor a ser creditado. Gravar o registro com o saldo atualizado */
			case 'c': CreditaCliente(&sArquivos, &sFindCli); break; 
			/* d: entre com conta e valor a ser debitado. Gravar o registro com o saldo atualizado */	
			case 'd': DebitaCliente(&sArquivos, &sFindCli);  break;  
			/* i: entre com conta, nome e saldo. Gravar o registro no arquivo auxiliar */			
			case 'i': IncluiCliente(&sArquivos, &sFindCli);  break;  
			/* e: entre com a conta. Alterar o campo valido */
			case 'e': ExcluiCliente(&sArquivos, &sFindCli);  break;  
		}
	}
	/* fecha arquivos utilizados pela aplicacao; */
	FechaArquivos(&sArquivos);
	printf("\nFinalizado!\n");
    return(0);
}
