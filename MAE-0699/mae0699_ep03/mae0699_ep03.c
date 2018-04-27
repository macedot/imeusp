/******************************************************************************
 * Nome  : Thiago Pinheiro de Macedo
 * N USP : 5124272
 ******************************************************************************/
/******************************************************************************
 * MAE0699 - Tópicos de Probabilidade e Estatística
 * Prof.: José Carlos Simon de Miranda
 * Exercicio de Implementação #03 (Sem Nome)
 ******************************************************************************/

/******************************************************************************
 * Definicoes internas
 ******************************************************************************/
#define  USE_SFMT      (1) /* Utiliza SFMT para geracao de numeros aleatorios */

/******************************************************************************
 * Includes da aplicacao;
 ******************************************************************************/
#if defined(WIN32)
#  include <windows.h>
#endif

#if USE_SFMT
#  include "SFMT.h"
#else
/* retirado de SFMT.h */
# if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#   include <inttypes.h>
# elif defined(_MSC_VER)
    typedef unsigned int uint32_t;
    typedef unsigned long long uint64_t;
#   define inline
# else
#   include <inttypes.h>
# endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

/******************************************************************************
 * Constantes da aplicacao
 ******************************************************************************/
#define NUM_DIMENSION     (3)
#define NUM_DIMENSION_1   (2)

#define VOLUME_ESFERA     (4.18879020478639098)  /* ((4.0/3.0) * PI) */
#define VOLUME_ELIPSOIDE  (4.23067810683425489)  /* (1.01 * VOLUME_ESFERA) */
#define INTEGRAL_ESFERA   (-1.396263393096121)   /* via metodo numerico; */

/******************************************************************************
 * Constantes para geracao de variaveis com distribuicao Gaussiana (Leva)
 ******************************************************************************/
#define GAUSSIAN_S              0.449871
#define GAUSSIAN_T             -0.386595
#define GAUSSIAN_A              0.19600
#define GAUSSIAN_B              0.25472
#define GAUSSIAN_R1             0.27597
#define GAUSSIAN_R2             0.27846

#define MY_ROOT_N(x, n)         (exp(log(x) / (double)(n))))
#define FACTOR_CALC(u, s)       (exp((log(u) / 3) - (log(s) / 2.0)))

/*
-> o fator de correcao para o algoritmo para gerar pontos na esfera 
   (Box-Muller)eh dado por :

        raiz_n(u)        u ^ (1 / 3)                 u ^ (1 / 3)
   f = ------------ = ------------------ = --------------------- = 
         || x ||       sqrt(sumSquareX)     sumSquareX ^ (1 / 2)

        (exp(log(     u      ) / 3.0))
     = ------------------------------- = 
        (exp(log( sumSquareX ) / 2.0))

             log(   u   )     log( sumSquareX )      
     = exp( -------------- - ------------------- ) = 
                 3.0                 2.0             
*/

/******************************************************************************
 * Tipos internos da aplicação
 ******************************************************************************/
typedef struct _vector {
  double  fCoord[NUM_DIMENSION];
  double  fSumSquare;
} Vetor;

typedef struct _points {
  Vetor*    pVetores;    /* vetor aplicado na funcao que resulta... */
  double*   pPontos;     /* ... no respectivo ponto; */
  double    fAverage;
  double    fVariance;
  uint32_t  nSize;
} Elemento;

/******************************************************************************
 * limites para geração dos ensaios
 ******************************************************************************/
uint32_t  NumPtsTheta    = 100;       /* Pontos para estimet theta; */
uint32_t  NumPtsInt      = 100000;    /* Pontos para estimar a integral; */
uint32_t  NumSimTheta    = 10;        /* Repeticoes da simulacao de theta; */
uint32_t  NumSimInt      = 100;       /* Repeticoes da simulacao da integral; */
BOOL      bCalcIntSphere = FALSE;

/******************************************************************************
 * void inicRNG(void)
 ******************************************************************************/
uint32_t getTime(void)
{
#if defined(__GNUC__)
  return (uint32_t)(1000 * (clock() /  (double)CLOCKS_PER_SEC));
#else
  return (uint32_t)GetTickCount();
#endif
}
/******************************************************************************
 * void inicRNG(void)
 ******************************************************************************/
void inicRNG(void)
{
#if USE_SFMT
  init_gen_rand((uint32_t)GetTickCount());
#else
  srand((uint32_t)clock());
#endif
}

/******************************************************************************
 * static double randUniform(void)
 ******************************************************************************/
static double randUniform(void)
{
#if USE_SFMT
  return ((double)genrand_res53());
#else
  return (double)(rand() / (double)RAND_MAX);
#endif /* USE_SFMT */
}

/******************************************************************************
 * static double getVariance(Elemento* pElemX)
 ******************************************************************************/
static double getVariance(Elemento* pElemX)
{
  uint32_t i;
  double fAux;

  pElemX->fVariance = 0.0;
  for(i = 0; i < pElemX->nSize; i++)
  {
    fAux = (pElemX->pPontos[i] - pElemX->fAverage);
    pElemX->fVariance += (fAux * fAux);
  }
  /* Desconta um grau de liberdade porque jah estimamos a media... =( */
  pElemX->fVariance /= (double)(pElemX->nSize - 1);
  return pElemX->fVariance;
}

/******************************************************************************
 * static double getCovariance(Elemento* pElemX, Elemento* pElemY)
 ******************************************************************************/
static double getCovariance(Elemento* pElemX, Elemento* pElemY)
{
  uint32_t i;
  double fCov;

  fCov = 0.0;

  if(pElemX->nSize != pElemY->nSize) 
    return fCov; /* Incompativel... */

  for(i = 0; i < pElemX->nSize; i++)
  {
    fCov += (pElemX->pPontos[i] - pElemX->fAverage) * (pElemY->pPontos[i] - pElemY->fAverage);
  }

  /* Aqui nao desconta um grau de liberdade... =0) */
  fCov /= (double)pElemX->nSize;

  return fCov;
}

/* Ratio method (Kinderman-Monahan); see Knuth v2, 3rd ed, p130.
 * K+M, ACM Trans Math Software 3 (1977) 257-260.
 *
 * [Added by Charles Karney] This is an implementation of Leva's
 * modifications to the original K+M method; see:
 * J. L. Leva, ACM Trans Math Software 18 (1992) 449-453 and 454-455. */
/******************************************************************************
 * static double randGaussian(void)
 ******************************************************************************/
static double randGaussian(void)
{
  double u, v, x, y, Q;

  /* This loop is executed 1.369 times on average  */
  do {
      /* Generate a point P = (u, v) uniform in a rectangle enclosing
         the K+M region v^2 <= - 4 u^2 log(u). */
      /* u in (0, 1] to avoid singularity at u = 0 */
      u = 1.0 - randUniform();
      /* v is in the asymmetric interval [-0.5, 0.5).  However v = -0.5
         is rejected in the last part of the while clause.  The
         resulting normal deviate is strictly symmetric about 0
         (provided that v is symmetric once v = -0.5 is excluded). */
      v = randUniform() - 0.5;
      /* Constant 1.7156 > sqrt(8/e) (for accuracy); but not by too
         much (for efficiency). */
      v *= 1.7156;
      /* Compute Leva's quadratic form Q */
      x = u - GAUSSIAN_S;
      y = fabs(v) - GAUSSIAN_T;
      Q = x * x + y * (GAUSSIAN_A * y - GAUSSIAN_B * x);
      /* Accept P if Q < r1 (Leva) */
      /* Reject P if Q > r2 (Leva) */
      /* Accept if v^2 <= -4 u^2 log(u) (K+M) */
      /* This final test is executed 0.012 times on average. */
  } while (Q >= GAUSSIAN_R1 && (Q > GAUSSIAN_R2 || v * v > -4 * u * u * log (u)));
  /* Return slope */
  return (v / u);
}

/******************************************************************************
 * static double randVectorSphere(Vetor* pVetor)
 ******************************************************************************/
static double randVectorSphere(Vetor* pVetor)
{
  uint32_t i;
  double  fFactor;

  pVetor->fSumSquare = 0.0;
  /* gera o vetor no espaco desejado */
  for(i = 0; (i < NUM_DIMENSION); i++)
  {
    pVetor->fCoord[i] = randGaussian();
    pVetor->fSumSquare += (pVetor->fCoord[i] * pVetor->fCoord[i]);
  }

  /* calcula fator de distancia */
  /* fFactor = pow(randUniform(), 1.0/3.0) / sqrt(pVetor->fSumSquare); */
  fFactor = FACTOR_CALC(randUniform(), pVetor->fSumSquare);
  
  /* projeta na supeficie, escalando o resultado 
     pelo multiplicador da distancia do ponto a origem; */
  pVetor->fSumSquare = 0.0;
  for(i = 0; (i < NUM_DIMENSION); i++)
  {
    pVetor->fCoord[i] *= fFactor;
    pVetor->fSumSquare += (pVetor->fCoord[i] * pVetor->fCoord[i]);
  }
  return pVetor->fSumSquare;
}

/******************************************************************************
 * static double randVectorElipsoide(Vetor* pVetor)
 * -> Gera um ponto no "Esferóide Prolato", a partir de um ponto na esfera;
 * -> Veja : 
 * http://www.thefreedictionary.com/prolate
 * http://pt.wikipedia.org/wiki/Esfer%C3%B3ide
 * http://en.wikipedia.org/wiki/Prolate_spheroid
 * http://en.wikipedia.org/wiki/Spheroid
 ******************************************************************************/
static double randVectorElipsoide(Vetor* pVetor)
{
  randVectorSphere(pVetor);
  /* Recalcula soma dos quadrados (1) */
  pVetor->fSumSquare -= (pVetor->fCoord[NUM_DIMENSION_1] * pVetor->fCoord[NUM_DIMENSION_1]);
  /* Transforma ultima coordenada; */
  pVetor->fCoord[NUM_DIMENSION_1] *= 1.01;
  /* Recalcula soma dos quadrados (2) */
  pVetor->fSumSquare += (pVetor->fCoord[NUM_DIMENSION_1] * pVetor->fCoord[NUM_DIMENSION_1]);
  return pVetor->fSumSquare;
}

/******************************************************************************
 * static double randVectorElipsoide(Vetor* pVetor)
 * -> Gera um ponto no "Esferóide Prolato", a partir de um ponto na esfera;
 * -> Veja : 
 * http://www.thefreedictionary.com/prolate
 * http://pt.wikipedia.org/wiki/Esfer%C3%B3ide
 * http://en.wikipedia.org/wiki/Prolate_spheroid
 * http://en.wikipedia.org/wiki/Spheroid
 ******************************************************************************/
static double randVectorElipsoideEx(Vetor* pVetor)
{
  /* Recalcula soma dos quadrados (1) */
  pVetor->fSumSquare -= (pVetor->fCoord[NUM_DIMENSION_1] * pVetor->fCoord[NUM_DIMENSION_1]);
  /* Transforma ultima coordenada; */
  pVetor->fCoord[NUM_DIMENSION_1] *= 1.01;
  /* Recalcula soma dos quadrados (2) */
  pVetor->fSumSquare += (pVetor->fCoord[NUM_DIMENSION_1] * pVetor->fCoord[NUM_DIMENSION_1]);
  return pVetor->fSumSquare;
}

/******************************************************************************
 * int inicPoints(Elemento* pElem, uint32_t nSize)
 ******************************************************************************/
int inicPoints(Elemento* pElem, uint32_t nSize)
{
  memset(pElem, 0, sizeof(Elemento));

  pElem->pPontos = (double*)malloc(nSize * sizeof(double));
  if(pElem->pPontos == NULL) 
    return FALSE;

  pElem->pVetores = (Vetor*)malloc(nSize * sizeof(Vetor));
  if(pElem->pVetores == NULL) 
    return FALSE;

  pElem->nSize  = nSize;
  return TRUE;
}

/******************************************************************************
 * static void freePoints(Elemento* pElem)
 ******************************************************************************/
static void freePoints(Elemento* pElem)
{
  if(pElem->pPontos != NULL)
  {
    free(pElem->pPontos);
  }
  if(pElem->pVetores != NULL)
  {
    free(pElem->pVetores);
  }
  memset(pElem, 0, sizeof(Elemento));
}

/******************************************************************************
 * static void genRandPoints(Elemento* pElem, double (*funcGenRandVet)(Vetor*))
 ******************************************************************************/
static void genRandPoints(Elemento* pElem, double (*funcGenRandVet)(Vetor*))
{
  uint32_t  i;

  pElem->fAverage = 0.0;
  for(i = 0; i < pElem->nSize; i++)
  {
    /* Funcao aplicada em um vetor aleatorio; */
    pElem->pPontos[i] = log(sqrt(funcGenRandVet(&(pElem->pVetores[i]))));
    pElem->fAverage += pElem->pPontos[i];
  }
  pElem->fAverage /= (double)pElem->nSize;
}

/******************************************************************************
 * int main(int argc, char* argv[])
 ******************************************************************************/
int main(int argc, char* argv[])
{
  uint32_t  i;
  Elemento  sElemSphere;
  Elemento  sElemElipsoide;
  double    fIntSphere;
  double    fIntElipsoide;
  double    fTheta;

  inicRNG();

  /****************************************************************/
  /****************************************************************/
  for(i = 0; i < 70; i++) printf("=");
  printf("\nCalculo da Integral via Monte Carlo com Variavel de Controle\n");
  for(i = 0; i < 70; i++) printf("=");
  printf("\n\n");
  /****************************************************************/
  /****************************************************************/
  for(i = 1; i < (uint32_t)argc; i++)
  {
    if(*argv[i] == '-')
    {
      switch(toupper(*(argv[i]+1)))
      {
        case 'I':
        {
          bCalcIntSphere = TRUE;
          break;
        }
        case 'P': /* Pontos para simulacao... */
        {
          switch(toupper(*(argv[i]+2)))
          {
            case 'T': /* ... de theta; */
              NumPtsTheta = (uint32_t)atol(argv[i]+3);
              if(NumPtsTheta <= 0)
              {
                printf("Numeros de ponto para Theta nao eh valido! =P\n");
                return 1;
              }
              break;
            case 'I': /* ... da integral; */
              NumPtsInt = (uint32_t)atol(argv[i]+3);
              if(NumPtsTheta <= 0)
              {
                printf("Numeros de ponto para a Integral nao eh valido! =P\n");
                return 1;
              }
              break;
          }
          break;
        }
        case 'S': /* Simulacoes para estimar... */
        {
          switch(toupper(*(argv[i]+2)))
          {
            case 'T': /* ... theta; */
              NumSimTheta = (uint32_t)atol(argv[i]+3);
              if(NumSimTheta <= 0)
              {
                printf("Numeros de Repeticoes para estimar Theta nao eh valido! =P\n");
                return 1;
              }
              break;
            case 'I': /* ... a integral; */
              NumSimInt = (uint32_t)atol(argv[i]+3);
              if(NumSimTheta <= 0)
              {
                printf("Numeros de Repeticoes para estimar a Integral nao eh valido! =P\n");
                return 1;
              }
              break;
          }
          break;
        }
      }
    }
  }
  /****************************************************************/
  /****************************************************************/
  printf("Parametros Operacionais:\n");
  printf("\tPontos para estimar Theta .....: %ld\n", NumPtsTheta);
  printf("\tPontos para estimar a Integral : %ld\n", NumPtsInt  );
  printf("\tSimulacoes de Theta ...........: %ld\n", NumSimTheta);
  printf("\tSimulacoes da Integral ........: %ld\n", NumSimInt  );
  printf("\n");
  /****************************************************************/
  /****************************************************************/
  if(!inicPoints(&sElemSphere, NumPtsTheta))
  {
    return 1;
  }
  if(!inicPoints(&sElemElipsoide, NumPtsTheta))
  {
    freePoints(&sElemSphere);
    return 1;
  }
  /****************************************************************/
  /****************************************************************/
  printf("Estimando valor de Theta...");
  fTheta = 0.0;
  for(i = 0; i < NumSimTheta; i++)
  {
    /* Gera pontos na esfera e pontos no elipsoide a partir destes (alta correlacao); */
    genRandPoints(&sElemSphere , randVectorSphere );        /* Y */
    memcpy(sElemElipsoide.pVetores, sElemSphere.pVetores, sElemSphere.nSize * sizeof(Vetor));
    genRandPoints(&sElemElipsoide, randVectorElipsoideEx);  /* Z */
    /* Obtem a variancia dos pontos; */
    fTheta += getCovariance(&sElemElipsoide, &sElemSphere) / getVariance(&sElemSphere);
  }
  fTheta /= (double)NumSimTheta;
  printf(" [ %.16f ]\n\n", fTheta);
  /****************************************************************/
  /****************************************************************/
  freePoints(&sElemSphere);
  freePoints(&sElemElipsoide);
  /****************************************************************/
  /****************************************************************/
  if(!inicPoints(&sElemSphere, NumPtsInt))
  {
    return 1;
  }
  if(!inicPoints(&sElemElipsoide, NumPtsInt))
  {
    freePoints(&sElemSphere);
    return 1;
  }
  /****************************************************************/
  /****************************************************************/
  if(bCalcIntSphere)
  {
    printf("Estimando valor da Integral na Esfera...");
    fIntSphere = 0.0;
    for(i = 0; i < NumSimInt; i++)
    {
      genRandPoints(&sElemSphere, randVectorSphere); /* Y */
      fIntSphere += sElemSphere.fAverage;
    }
    fIntSphere /= (double)NumSimInt;
    fIntSphere *= VOLUME_ESFERA;
    printf(" [ %.16f ]\n\n", fIntSphere);
  }
  else
  {
    fIntSphere = INTEGRAL_ESFERA;
    printf("Valor da Integral na Esfera...");
    printf(" [ %.16f ]\n\n", fIntSphere);
  }
  /****************************************************************/
  /****************************************************************/
  printf("Estimando valor da Integral no Elipsoide...");
  fIntElipsoide = 0.0;
  for(i = 0; i < NumSimInt; i++)
  {
    genRandPoints(&sElemElipsoide, randVectorElipsoide); /* Z */
    genRandPoints(&sElemSphere   , randVectorSphere   ); /* Y */
    fIntElipsoide += sElemElipsoide.fAverage - fTheta * (VOLUME_ESFERA * sElemSphere.fAverage - fIntSphere);
  }
  fIntElipsoide /= (double)NumSimInt;
  fIntElipsoide *= VOLUME_ELIPSOIDE;
  printf(" [ %.16f ]\n\n", fIntElipsoide);
  /****************************************************************/
  /****************************************************************/
  printf(">>> Diferenca para a integral na Esfera: %.16f\n\n", fIntElipsoide - fIntSphere);
  /****************************************************************/
  /****************************************************************/
  freePoints(&sElemElipsoide);
  freePoints(&sElemSphere);

  return 0;
}
