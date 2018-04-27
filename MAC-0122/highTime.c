/***********************************************************
 * highTime.c
 * 
 * @version 1.0 30 Nov 2005
 ***********************************************************/

/***********************************************************
 * Defines internos
 **********************************************************/
/* #define LINUX */

/* 
- ATENCAO: Frequencia do seu processador (arredonde); Ex: 1500 Mhz; 
Deve ser alterada para cada caso !! (porem, numeros na ordem de 
10^9, 10^10 convergem os resultados para valores razoaveis
(mas so expressam os segundos se o valor for o clock EXATO);
*/
#define CLOCK_PROC   1808250000.0

/***********************************************************
 * Includes da aplicacao
 **********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
  unsigned int nLow=0, nHigh=0;
  
#ifdef LINUX
  /* modo linux */
  __asm__ __volatile__("rdtsc" : "=a" (nLow), "=d" (nHigh));
#else
  /* modo nao linux */
  __asm
  {
    rdtsc
    mov nHigh, edx
    mov nLow, eax
  }
#endif
  *pLow  = nLow;
  *pHigh = nHigh;
}
/***********************************************************
 * Funcao principal
 **********************************************************/
/*----------------------------------------------------------
Para saber a frquencia "exata" de seu processador:
Windows: http://www.cpuid.com/download/cpu-z-131.zip
Linux  : cat /proc/cpuinfo (thx Mestre Rodolpho)
----------------------------------------------------------*/
int main(void)
{
  unsigned int nLowA, nHighA;
  unsigned int nLowB, nHighB;
  double fTime   = 0.0;

  myClock(&nLowA, &nHighA);
  /* 
    .....
    aqui vai o codigo o qual desejamos medir o tempo 
    .....
  */
  myClock(&nLowB, &nHighB);
  fTime = ((double)((nLowB-nLowA) | ((nHighB-nHighA) << 32)) / CLOCK_PROC);
  printf("Tempo: %0.10f s;\n",fTime);
  return(0);
}

