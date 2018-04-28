/******************************************************************************
 * Nome  : Thiago Pinheiro de Macedo
 * N USP : 5124272
 ******************************************************************************/
/******************************************************************************
 * MAE0699 - Tópicos de Probabilidade e Estatística
 * Prof.: José Carlos Simon de Miranda
 * Exercicio de Implementação #02 (Sem Nome)
 ******************************************************************************/

/******************************************************************************
 * Definicoes internas
 ******************************************************************************/

/******************************************************************************
 * Includes da aplicacao;
 ******************************************************************************/
#if defined(WIN32)
	#include <windows.h>
#else
	#include <string.h>
#endif

#include "dSFMT/dSFMT.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#ifndef TRUE
	#define TRUE (1)
#endif

#ifndef FALSE
	#define FALSE (0)
#endif

/******************************************************************************
 * Constantes da aplicacao
 ******************************************************************************/
#define NUM_REGION              9
#define NUM_DIMENSION           7
#define NUM_DIMENSION_1         6
#define DIMENSION_INV           0.142857142857    /* (1.0 / 7.0) */
#define VOLUME_HYPERSPHERE_R7   4.72476597        /* ~ 16 * PI^3 / 105; */

/*
  Min.   :0.1002
  1st Qu.:0.5726
  Median :0.7455
  Mean   :0.7120
  3rd Qu.:0.8811
  Max.   :1.0000
*/

#define PARTE2_REGIAO_0         0.1                 /* (por causa da regiao 6) */
#define PARTE2_REGIAO_1         0.75                /* 0.7578582832551990 */
#define PARTE2_REGIAO_2         0.87                /* 0.8705505632961241 */
#define PARTE2_REGIAO_3         0.94                /* 0.9440875112949019 */
#define PARTE2_REGIAO_4         1.0                 /* (do enunciado) */

#ifndef M_PI
	#define M_PI                  3.1415926535897932    /* ~ pi */
#endif

#ifndef M_2PI
	#define M_2PI                 6.2831853071795864   /* ~ 2*pi */
#endif

#ifndef M_SQRT2
	#define M_SQRT2               1.4142135623730950   /* ~ sqrt(2.0) */
#endif

#define PI_OVER_7               0.4487989505128276 /* ~(pi/7) */

/******************************************************************************
 * Constantes para geracao de variaveis com distribuicao Gaussiana (Leva)
 ******************************************************************************/
#define GAUSSIAN_S              0.449871
#define GAUSSIAN_T             -0.386595
#define GAUSSIAN_A              0.19600
#define GAUSSIAN_B              0.25472
#define GAUSSIAN_R1             0.27597
#define GAUSSIAN_R2             0.27846

/******************************************************************************
 * Tipos internos da aplicação
 ******************************************************************************/
typedef struct _vector {
	double  fCoord[NUM_DIMENSION];
	double  fSumSquare;
} Vetor;

typedef struct _region {
	uint32_t  nIdx;   /* indice da regiao */
	int (*fpCheckRegion)(Vetor* pVetor);  /* funcao de teste destra regiao */
	uint32_t  nExit;  /* numeros de pontos descartados nesta regiao */
} Region;

/******************************************************************************
 * limites para geração dos ensaios
 ******************************************************************************/
char      szRegion[NUM_REGION+1] = "123456789";
// char      szRegion[NUM_REGION+1] = "23967458"; /* ordem "otima" */
// char      szRegion[NUM_REGION+1] = "239674"; /* 5,8 nao descartam pontos */
char      verboseMode = 0;
uint32_t  NumPts    = 100000;
uint32_t  NumRepeat = 1;
dsfmt_t   dsfmt;

/******************************************************************************
 * void inicRNG(void)
 ******************************************************************************/
inline static uint32_t getTime(void)
{
#if defined(__GNUC__)
	return (uint32_t)(1000 * (clock() / (double)CLOCKS_PER_SEC));
#else
	return (uint32_t)GetTickCount();
#endif
}
/******************************************************************************
 * void inicRNG(void)
 ******************************************************************************/
inline static void inicRNG(void)
{
	dsfmt_init_gen_rand(&dsfmt, (uint32_t)time(NULL));
}

/******************************************************************************
 * static double randUniform(void)
 ******************************************************************************/
inline static double randUniform(void)
{
	return dsfmt_genrand_close_open(&dsfmt);
}

/******************************************************************************
 * static double randUniform(void)
 ******************************************************************************/
static double getVariance(double* pVal, double fMean, uint32_t n)
{
	register uint32_t i;
	double fVariance = 0.0;
	double fAux;

	for(i = 0; i < n; i++) {
		fAux = (pVal[i] - fMean);
		fVariance += (fAux * fAux);
	}
	fVariance /= (double)(n - 1);
	return fVariance;
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
static void randVectorSphere(Vetor* pVetor)
{
	register uint32_t i;
	double  fFactor;

	pVetor->fSumSquare = 0.0;
	/* gera o vetor no espaco desejado */
	for(i = 0; (i < NUM_DIMENSION); i++) {
		pVetor->fCoord[i] = randGaussian();
		pVetor->fSumSquare += (pVetor->fCoord[i] * pVetor->fCoord[i]);
	}
	/* calcula fator de distancia */
	fFactor = pow(randUniform(), DIMENSION_INV) / sqrt(pVetor->fSumSquare);
	/* projeta na supeficie, escalando o resultado
	pelo multiplicador da distancia do ponto a origem; */
	pVetor->fSumSquare = 0.0;
	for(i = 0; (i < NUM_DIMENSION); i++) {
		pVetor->fCoord[i] *= fFactor;
		pVetor->fSumSquare += (pVetor->fCoord[i] * pVetor->fCoord[i]);
	}
}

/******************************************************************************
 * static double randVectorIntegralRegion(Vetor* pVetor, double r0, double r1)
 Como Visto em sala:
 "para  r0 < r < r1 basta multiplicar o ponto gerado por
  raiz-enesima(r0^n + u*(r1^n - r0^n)), com u ~ U[0,1]."
 ******************************************************************************/
static void randVectorIntegralRegion(Vetor* pVetor, double r0, double r1)
{
	register uint32_t i;
	double  fFactor;
	double  fPow_r0;

	pVetor->fSumSquare = 0.0;
	/* gera o vetor no espaco desejado */
	for(i = 0; i < NUM_DIMENSION; i++) {
		pVetor->fCoord[i] = randGaussian();
		pVetor->fSumSquare += (pVetor->fCoord[i] * pVetor->fCoord[i]);
	}

	/* calcula fator de distancia */
	fPow_r0  = pow(r0, NUM_DIMENSION);
	fFactor  = pow(r1, NUM_DIMENSION) - fPow_r0;
	fFactor *= randUniform();
	fFactor += fPow_r0;
	fFactor  = pow(fFactor, DIMENSION_INV);
	fFactor /= sqrt(pVetor->fSumSquare);

	/* projeta na supeficie, escalando o resultado
	pelo multiplicador da distancia do ponto a origem; */
	pVetor->fSumSquare = 0.0;
	for(i = 0; i < NUM_DIMENSION; i++) {
		pVetor->fCoord[i] *= fFactor;
		pVetor->fSumSquare += (pVetor->fCoord[i] * pVetor->fCoord[i]);
	}
}
/******************************************************************************
 * static int checkRegion1(Vetor* pVetor)
 ******************************************************************************/
inline static int checkRegion1(Vetor* pVetor)
{
	return (pVetor->fSumSquare < 1.0);
}

/******************************************************************************
 * static int checkRegion2(Vetor* pVetor)
 ******************************************************************************/
static int checkRegion2(Vetor* pVetor)
{
	register uint32_t i;
	double fAux = -0.5;

	for(i = 0; (i < NUM_DIMENSION_1); i++)
		fAux += pow(fabs(pVetor->fCoord[i]), i+1);

	return (pVetor->fCoord[NUM_DIMENSION_1] >= fAux);
}

/******************************************************************************
 * static int checkRegion3(Vetor* pVetor)
 ******************************************************************************/
static int checkRegion3(Vetor* pVetor)
{
	register uint32_t i;
	double fSum = 0.0;

	for(i = 0; (i < NUM_DIMENSION); i++)
		fSum += pVetor->fCoord[i];

	return (fSum <= 1.0);
}

/******************************************************************************
 * static int checkRegion4(Vetor* pVetor)
 ******************************************************************************/
static int checkRegion4(Vetor* pVetor)
{
	register uint32_t i;
	double  fAux = 0.0;
	double  fAbs;

	for(i = 0; (i < NUM_DIMENSION_1); i++) {
		fAbs = fabs(pVetor->fCoord[i]);
		fAux += pow(fAbs, 1.0 + fAbs);
	}
	fAux = 1.0 - (fabs(fAux) / 10.0);
	return (pVetor->fCoord[NUM_DIMENSION_1] <= fAux);
}

/******************************************************************************
 * static int checkRegion5(Vetor* pVetor)
 ******************************************************************************/
inline static int checkRegion5(Vetor* pVetor)
{
	return ((pVetor->fCoord[0] * pVetor->fCoord[1]) < 0.49);
}

/******************************************************************************
 * static int checkRegion6(Vetor* pVetor)
 ******************************************************************************/
inline static int checkRegion6(Vetor* pVetor)
{
	return (pVetor->fSumSquare > 0.1);
}

/******************************************************************************
 * static int checkRegion7(Vetor* pVetor)

  (Método via Algebra Linear)

Time(2062);
Regiao 7 :        831;
NumPts :      1000000;
NumHit :       999169;
Ratio  : 0.9991690000;
Volume : 4.7208396895;


          || x - p ||
               |
            /\ |       .
           /  \|     .
          /   /\  .
         /   |. o
      x /     .   p
       /    .
      /   .
     /  .  s
    / .
   /.
  o

          o
         /|
        / |
       /  |
    x /   | || x - p || > E
     /    |
    /    _|
   /    |.v
  o------>o--------->
  |   p   |         |
  |         s       |

  E:= ("epson") Distancia de S ate extremo da regiao de rejeição;
  s:= vetor construido em S;
  x:= vetor simulado;
  p:= projecao do "vetor simulado" em S;

  Definimos:
    x := (x_1, ... , x_6,                          x_7)
    s := (x_1, ... , x_6, sqrt(Sum([1..6], (x_i ^ 2))))
      := (s_1, ... , s_6,                          s_7)
    p :=  alpha * (s_1, ... ,                      s_7)
      := (alpha * s_1, ... ,               alpha * s_7)
 ******************************************************************************/
static int checkRegion7(Vetor* pVetorX)
{
	register uint32_t i;
	Vetor   sVetorS;
	Vetor   sVetorP;
	double  fAlpha;
	double  f_sTx;
	double  fDist;

	/* construcao de S */
	memcpy(&sVetorS, pVetorX, sizeof(Vetor));
	sVetorS.fSumSquare = 0.0;
	for(i = 0; i < NUM_DIMENSION_1; i++)
		sVetorS.fSumSquare += (sVetorS.fCoord[i] * sVetorS.fCoord[i]);

	sVetorS.fCoord[NUM_DIMENSION_1] = sqrt(sVetorS.fSumSquare);
	sVetorS.fSumSquare += (sVetorS.fCoord[NUM_DIMENSION_1] * sVetorS.fCoord[NUM_DIMENSION_1]);

	/* construcao de P */
	f_sTx = 0.0;
	for(i = 0; i < NUM_DIMENSION; i++)
		f_sTx += (sVetorS.fCoord[i] * pVetorX->fCoord[i]);

	fAlpha = (f_sTx / sVetorS.fSumSquare);
	for(i = 0; i < NUM_DIMENSION; i++)
		sVetorP.fCoord[i] = (fAlpha * sVetorS.fCoord[i]);

	/* distancia de X a P */
	fDist = 0;
	for(i = 0; i < NUM_DIMENSION; i++) {
		fAlpha = (pVetorX->fCoord[i] * sVetorP.fCoord[i]);
		fDist += (fAlpha * fAlpha);
	}

	return (fDist > 0.000001);
}
/******************************************************************************
 * static int checkRegion7_Ex(Vetor* pVetor)
 (Metodo via calculo - Mostrado em sala - Prof. Simon)

Time(1968);
Regiao 7 :        817;
NumPts :      1000000;
NumHit :       999183;
Ratio  : 0.9991830000;
Volume : 4.7209058362;

 ******************************************************************************/
static int checkRegion7_Ex(Vetor* pVetorX)
{
	double  fDist;

	fDist = pVetorX->fSumSquare - (pVetorX->fCoord[NUM_DIMENSION_1] * pVetorX->fCoord[NUM_DIMENSION_1]);
	fDist = sqrt(fDist) - fabs(pVetorX->fCoord[NUM_DIMENSION_1]);
	fDist = fabs(fDist) / M_SQRT2;
	return (fDist > 0.001);
}
/******************************************************************************
 * static int checkRegion8(Vetor* pVetor)
 ******************************************************************************/
static int checkRegion8(Vetor* pVetor)
{
	register uint32_t  i;
	double t;
	double fDist;
	double fMax;
	double step = (M_2PI / 100.0); /* Passo dos valores de t; */
	double fMinMax = 2.0; /* "Sempre" sera menor que 2.0; */

	for(t = 0.0; t < M_2PI; t += step) {
		fMax = 0;
		for(i = 0; (i < NUM_DIMENSION); i++) {
			fDist = fabs(pVetor->fCoord[i] - sin((i * t) + (i - 1) * PI_OVER_7));
			if(fDist > fMax)
				fMax = fDist;
		}
		if(fMax < fMinMax)
			fMinMax = fMax;
	}
	return (fMinMax > 0.01);
}
/******************************************************************************
 * static int checkRegion9(Vetor* pVetor)
 ******************************************************************************/
static int checkRegion9(Vetor* pVetor)
{
	register uint32_t  i;
	double fAux;
	double fMin = 1.0;

	for(i = 0; (i < NUM_DIMENSION); i++) {
		/* Obtem a parte fracionaria (que eh sempre >0) do valor de Xi;*/
		fAux  = fabs(1000.0 * pVetor->fCoord[i]);
		fAux -= floor(fAux);
		if(fAux < fMin)
			fMin = fAux;
	}
	return (fMin >= 0.01);
}

/******************************************************************************
 * int main(int argc, char* argv[])
 ******************************************************************************/
int main(int argc, char* argv[])
{
	register uint32_t  i;
	register uint32_t  j;
	register uint32_t  k;
	const double RadiusBola[] = {
		PARTE2_REGIAO_0,
		PARTE2_REGIAO_1,
		PARTE2_REGIAO_2,
		PARTE2_REGIAO_3,
		PARTE2_REGIAO_4
	};
	unsigned char      bExit;
	Region    sRegion[NUM_REGION];
	Vetor     sVetor;
	uint32_t  PointsGerRegio[4] = { 0, 0, 0, 0 };
	uint32_t  HitPerRegion[4] = { 0, 0, 0, 0 };
	uint32_t  NumRegion = 0;
	uint32_t  NumHit    = 0;
	uint32_t  nIdx      = 0;
	uint32_t  repeat    = 0;
	uint32_t  unTime;
	double*   pVal = NULL;
	double    fIntegralParte2;
	double    fIntegralRegion;
	double    fVarRegion;
	double    fRatio;
	double    fVolumeRegiao;
	double    ProbPerBola[4] = { 0.0, 0.0, 0.0, 0.0 };

	for(i = 1; i < (uint32_t)argc; i++) {
		if(*argv[i] == '-') {
			switch(toupper(*(argv[i]+1))) {
				case 'N':
					NumRepeat = (uint32_t)atol(argv[i]+2);
					break;
				case 'P':
					NumPts  = (uint32_t)atol(argv[i]+2);
					break;
				case 'R':
					strncpy(szRegion, argv[i]+2, NUM_REGION);
					szRegion[NUM_REGION] = '\0';
					break;
				case 'V':
					verboseMode = 1;
					break;
			}
		}
	}

	/* preenche estrutura de regioes a serem testadas; */
	NumRegion = 0;
	memset(&sRegion, 0, sizeof(sRegion));
	for(i = 0; (szRegion[i] != '\0') && (i < NUM_REGION); i++) {
		/* Verifica se o caracter eh um digito; */
		if(isdigit((int)szRegion[i])) {
			/* Obtem o indice que identifica a regiao; */
			nIdx = (szRegion[i] - '0');
			if((nIdx >= 1) && (nIdx <= NUM_REGION)) {
				/* Preenche membro da estrutura com os respectivos dados; */
				sRegion[NumRegion].nIdx = nIdx;
				switch(nIdx) {
					case 1: sRegion[NumRegion].fpCheckRegion = checkRegion1; break;
					case 2: sRegion[NumRegion].fpCheckRegion = checkRegion2; break;
					case 3: sRegion[NumRegion].fpCheckRegion = checkRegion3; break;
					case 4: sRegion[NumRegion].fpCheckRegion = checkRegion4; break;
					case 5: sRegion[NumRegion].fpCheckRegion = checkRegion5; break;
					case 6: sRegion[NumRegion].fpCheckRegion = checkRegion6; break;
					case 7: sRegion[NumRegion].fpCheckRegion = checkRegion7; break;
					case 8: sRegion[NumRegion].fpCheckRegion = checkRegion8; break;
					case 9: sRegion[NumRegion].fpCheckRegion = checkRegion9; break;
				}
				NumRegion++;
			}
		}
	}

	inicRNG();
	memset(&sVetor,0, sizeof(sVetor));

	/****************************************************************************\
	|*              Primeira Parte : Volume da Hyper-Esfera em R^7              *|
	\****************************************************************************/
	for(i = 0; i < 70; i++) printf("=");
	printf("\nPrimeira parte - Volume da Regiao\n");
	for(i = 0; i < 70; i++) printf("=");
	printf("\n");

	if (verboseMode) {
		printf("IDX| TIME ");
		for(j = 0; (j < NumRegion); j++)  printf("| REG%d " , sRegion[j].nIdx);
		printf("| TOTAL\n");
		printf("---+------");
		for(j = 0; (j < NumRegion); j++)  printf("+------");
		printf("+-------\n");
	}

	fRatio = 0.0;
	for(repeat = 0; repeat < NumRepeat; repeat++) {
		for(j = 0; (j < NumRegion); j++)
			sRegion[j].nExit = 0;

		NumHit = 0;
		memset(&HitPerRegion, 0, sizeof(HitPerRegion));

		unTime = getTime();
		for(i = 0; i < NumPts; i++) {
			bExit = FALSE;
			randVectorSphere(&sVetor);
			for(j = 0; !bExit && (j < NumRegion); j++) {
				if(!sRegion[j].fpCheckRegion(&sVetor)) {
					/* Ponto foi descartado... =( */
					sRegion[j].nExit++;
					bExit = TRUE;
				}
			}
			/* ponto nao foi descartado! =) */
			if(!bExit) {
				/* (para parte 2) verifica em qual das bolas ele acertou; */
				if(sVetor.fSumSquare > PARTE2_REGIAO_3)
					HitPerRegion[3]++;
				else if(sVetor.fSumSquare > PARTE2_REGIAO_2)
					HitPerRegion[2]++;
				else if(sVetor.fSumSquare > PARTE2_REGIAO_1)
					HitPerRegion[1]++;
				else
					HitPerRegion[0]++;
				/* incrementa numeros de pontos dentro da regiao desejada; */
				NumHit++;
			}
		}
		unTime = getTime() - unTime;

		if (verboseMode) {
			printf("%03u|%6u", repeat+1, unTime);
			for(j = 0; (j < NumRegion); j++)  printf("|%6u" , sRegion[j].nExit);
			printf("|%6u\n" , NumHit);
		}

		/* Acumula proporcoes de pontos nas bolas */
		for(j = 0; (j < 4); j++)
			ProbPerBola[j] += (HitPerRegion[j] / (double)NumHit);

		fRatio += (double)(NumHit / (double)NumPts);
	}

	/* Obtem a razao media dos pontos dentro das regioes; */
	fRatio /= (double)NumRepeat;

	if (verboseMode)
		printf("\n");

	fVolumeRegiao = VOLUME_HYPERSPHERE_R7 * fRatio;

	printf(">>> Points    : %u\n"   , NumPts);
	printf(">>> Avg Ratio : %.10f\n", fRatio);
	printf(">>> Volume    : %.10f\n", fVolumeRegiao);

	/****************************************************************************\
	|*   Segunda Parte (a) : Calculo da Integral por Amostragem Estratificada   *|
	|*                                                                          *|
	|* - http://en.wikipedia.org/wiki/Stratified_sampling                       *|
	|* - http://en.wikipedia.org/wiki/Monte_Carlo_integration                   *|
	|* - http://xbeams.chem.yale.edu/~batista/vaa/node47.html                   *|
	|*                                                                          *|
	\****************************************************************************/
	for(i = 0; i < 70; i++) printf("=");
	printf("\nSegunda parte - Calculo da Integral por Amostragem Estratificada\n");
	for(i = 0; i < 70; i++) printf("=");
	printf("\n");

	if (verboseMode) {
		printf("BOLA|  PROB  | POINT |   INTEGRAL   |   VARIANCE   \n");
		printf("----+--------+-------+--------------+--------------\n");
	}

	fIntegralParte2 = 0.0;
	for(k = 0; (k < 4); k++) {
		/* calcula "probabilidade" das bolas; */
		ProbPerBola[k] /= (double)NumRepeat;
		/* calcula numero de pontos para gerar em cada regiao; */
		PointsGerRegio[k] = (uint32_t)ceil(NumHit * ProbPerBola[k]);

		if (verboseMode)
			printf(" %02d | %2.4f | %5d | " , k+1, ProbPerBola[k], PointsGerRegio[k]);

		/* Gera pontos em cada "bola" e testa nas regioes */
		pVal = (double*)malloc(PointsGerRegio[k] * sizeof(double));
		NumHit = 0;
		fIntegralRegion = 0.0;
		for(i = 0; i < PointsGerRegio[k]; i++) {
			bExit = FALSE;
			randVectorIntegralRegion(&sVetor, RadiusBola[k], RadiusBola[k+1]);
			for(j = 0; !bExit && (j < NumRegion); j++)
				if(!sRegion[j].fpCheckRegion(&sVetor))
					bExit = TRUE; /* Ponto foi descartado... =( */
			/* ponto nao foi descartado! =) */
			if(!bExit) {
				pVal[NumHit] = (1.0 / sVetor.fSumSquare);
				fIntegralRegion += pVal[NumHit];
				NumHit++;
			}
		}

		/* Integral na regiao eh a media dos valores na "bola"; */
		fIntegralRegion /= (double)NumHit;
		/* Faz o produto pela "probabilidade" da "bola" */
		fIntegralRegion *= ProbPerBola[k];
		/* Integral da "parte 2 do LAB" eh a soma das integrais nas "bolas"; */
		fIntegralParte2 += fIntegralRegion;
		/* Calcula a variancia dos pontos na regiao; */
		fVarRegion = getVariance(pVal, fIntegralRegion, NumHit);

		free(pVal);

		if (verboseMode)
			printf("%-2.10f | %-2.10f\n", fIntegralRegion, fVarRegion);
	}

	if (verboseMode)
		printf("\n");

	/* Integral da parte 2 eh a soma das integrais nas bolas vezes o volume das regiao R (parte 1); */
	fIntegralParte2 *= fVolumeRegiao;

	printf(">>> Integral : %.10f\n" , fIntegralParte2);

	/****************************************************************************\
	|*   Segunda Parte (b) : Calculo da Integral por Amostragem Simplificada    *|
	\****************************************************************************/

	return 0;
}
