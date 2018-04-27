/***************************************************************************************************
 * Thiago Pinheiro de Macedo - N USP: 5124272
 * MAC 0212 - Laboratório de Computação
 * IME USP - BMAC - Noturno - Primeiro Semestre de 2006
 * Exercício Programa 2 - Prof. Marcilio - Entregar - 28Mai06
 **************************************************************************************************/
#ifndef MAC0212EP02_H
#	define MAC0212EP02_H

/***************************************************************************************************
 * Verifica se estamos rodando em ambiente MS-Windows;
 * Em caso negativo, o fonte foi padronizado para GNU/LINUX;
 **************************************************************************************************/
#if defined(WIN32) || defined(_WINDOWS) 
#	define EP_WINDOWS
#endif

/***************************************************************************************************
 * Includes da aplicacao
 **************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* includes distintos dos OSs */
#ifdef EP_WINDOWS
#	include <io.h>
#	include <conio.h>
#else
#	include <unistd.h>
#endif

/***************************************************************************************************
 * Defines internos
 **************************************************************************************************/
#define MAX_REGISTROS		10

#define NUMERO_USP			5124272

#ifndef RAND_MAX
#	define RAND_MAX			0x7fffffff					/* Maximo para geracao de valores random */
#endif

#ifndef TRUE
#	define TRUE				1
#endif

#ifndef FALSE
#	define FALSE			0
#endif

#ifndef _MAX_PATH
#	define _MAX_PATH		260							/* max. length of full pathname */
#endif

/* define indicador de cliente valido; */
#define CLIENTE_VALIDO		'v'

/* nomes dos arquivos para localizacao; */
#define ARQ_CLI_NOME        "ArqPrincipal"
#define ARQ_INC_NOME        "ArqInclusao"
#define ARQ_MER_NOME        "NovoArqPrincipal"
#define ARQ_TMP_NOME        "temp.dat"

/**************************************************************************************************
 * Definicao de tipos internos
 *************************************************************************************************/
/* evita o realinhamento das estruturas */
/*
#pragma pack( push )
#pragma pack( 1 )
*/

/* estrutura cliente, utilizada nos arquivos */
typedef struct cliente {
	int     nConta;        /* número da conta */
	char    szNome[30];    /* nome do cliente */
	double  fSaldo;        /* saldo bancário em R$ */
	char    cValido;       /* se branco então o registro é valido */
	char    szResto[85];   /* resto dos dados do cliente */
} CLIENTE; /* 128 bytes (alinhamento 1 byte); 136 bytes (alinhamento 4 bytes); */

/* estrutura para busca de cliente */
typedef struct findCliente {
	FILE*    fpArq;
	int      nPos;
	CLIENTE  sCliente;
} FINDCLI;

/* estrutura de arquivos */
typedef struct arquivos {
	FILE* fpArqCli;
	FILE* fpArqInc;
	FILE* fpArqMer;
} ARQUIVOS;

/* retorna alinhamento ao estado original */
/*
#pragma pack( pop )
*/

#endif /* MAC0212EP02_H */
