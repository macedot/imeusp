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
 * Gera numero aleatorio com distribuicao uniforme entre 0 e nLimite;
 **************************************************************************************************/
int GeraRand(int nLimite)
{
	int nRand;

	nRand = (int)((float)(nLimite + 1) * rand() / (RAND_MAX + 1.0)); 
	return(nRand);
}
/***************************************************************************************************
 * Gera um nome aleatorio
 **************************************************************************************************/
char* GeraNome(char szNome[]) 
{
	static char szNomeParte1[10][10] = {
		"Antonio", "Marcos", "Ciro"  , "Abel"   , "Alfredo", 
		"Carlos" , "Joao"  , "Moacir", "Orlando", "Paulo"
	};
	static char szNomeParte2[10][10] = {
		"Almeida", "Arcanjo", "Souza", "Araujo", "Monteiro", 
		"Cardoso", "Lima"   , "Silva", "Matias", "Setubal"
	};
	static char szNomeParte3[10][11] = {
		"Castro"    , "Leme"     , ""      , "de Matos", "da Silva",
		"dos Santos", "das Neves", "Cabral", "Gonzaga" , "da Costa"
	};
	int nIdx[3];
	int i;
	
	/* gera 3 indices aleatorios para as partes de nomes */
	for(i=0; i<3; i++)  
	{
		nIdx[i] = GeraRand(10);  
	}
	/* cria um nome aleatorio composto de 3 partes */
	sprintf(szNome, "%s %s %s", 
			szNomeParte1[nIdx[0]], szNomeParte2[nIdx[1]], szNomeParte3[nIdx[2]]);
	return(szNome);
}
/***************************************************************************************************
 * Preenche um registro Cliente com dados aleatorios
 **************************************************************************************************/
void GeraRegCliente(CLIENTE* pCliente, int nIndex) 
{
	/* numero de conta criado aleatoriamente, porem sequencial */
	pCliente->nConta = (nIndex * 100) + GeraRand(100); 
	pCliente->fSaldo = (rand() / 10.0);
	/* Cria nome do correntista */
	GeraNome(pCliente->szNome);
	/* marca registro como valido */
	pCliente->cValido = CLIENTE_VALIDO;
}
/***************************************************************************************************
 * Funcao Principal; Entry-point da aplicacao;
 **************************************************************************************************/
int main(void)
{
	CLIENTE  sCliente;
	FILE*    fpArqCli = NULL;
	int      i = 0;
	
	printf("Programa 00 - Criacao de base inicial de cliente\n");
	/* cria um novo arquivo principal para base de clientes */
	if((fpArqCli = fopen(ARQ_CLI_NOME,"w+b")) == NULL) 
	{
		printf("Nao foi possivel criar o arquivo de de Cliente;\n");
		return(1);
	}
	srand(NUMERO_USP); /* valor inicial para rand() */
	printf("Gerando arquivo de Cliente:\n");
	/* cria MAX_REGISTROS registros clientes */
	while(i < MAX_REGISTROS) 
	{
		i++;
		memset(&sCliente, 0, sizeof(CLIENTE));
		GeraRegCliente(&sCliente, i);
		if(fwrite(&sCliente, 1, sizeof(CLIENTE), fpArqCli) != sizeof(CLIENTE))
		{
			fclose(fpArqCli);
			printf("Erro gravando arquivo de cliente (%d);\n",i);
			return(2);
		}
		if((i % 100) == 0) 
		{
			printf("...%d", i);
		}
	}
	fclose(fpArqCli);
	printf("\nFinalizado!\n");
/*
	printf(" sCliente.nConta = %d\n", sizeof(sCliente.nConta));
	printf(" sCliente.szNome = %d\n", sizeof(sCliente.szNome));
	printf(" sCliente.fSaldo = %d\n", sizeof(sCliente.fSaldo));
	printf("sCliente.cValido = %d\n", sizeof(sCliente.cValido));
	printf("sCliente.szResto = %d\n", sizeof(sCliente.szResto));
	printf("        sCliente = %d\n", sizeof(sCliente));
	printf("  struct cliente = %d\n", sizeof(struct cliente));
	printf("         CLIENTE = %d\n", sizeof(CLIENTE));
	printf("             int = %d\n", sizeof(int));
	printf("            char = %d\n", sizeof(char));
	printf("          double = %d\n", sizeof(double));
*/
	return(0);
}
