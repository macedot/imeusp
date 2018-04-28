/******************************************************************************
 * Nome  : Thiago Pinheiro de Macedo
 * N USP : 5124272
 ******************************************************************************/
/******************************************************************************
 * MAE0699 - Tópicos de Probabilidade e Estatística
 * Prof.: José Carlos Simon de Miranda
 * Exercicio de Implementação #01 (Sem Nome)
 * Desenvolvido utilizando Visual C++ 2005 SP1
 ******************************************************************************/

/* Utiliza "SIMD oriented Fast Mersenne Twister(SFMT)" para rng uniformes; */
#define USE_SFMT

#ifdef __linux__
#include <ctype.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/time.h>
long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}
#else
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifdef USE_SFMT
#include "SFMT/SFMT.h"
#endif

#ifndef M_PI
const double M_PI  = 3.14159265358979323846264338328L;      /* pi */
#endif

#ifndef M_2PI
const double M_2PI = 6.28318530717958647692528676656L;      /* 2*pi */
#endif

#ifndef M_SQRT2
const double M_SQRT2 = 1.4142135623730950488016887242097L;   /* sqrt(2.0) */
#endif

/* Constantes para geracao de variaveis com distribuicao Gaussiana (Leva) */
const double GAUSSIAN_S    =  0.449871L;
const double GAUSSIAN_T    = -0.386595L;
const double GAUSSIAN_A    =  0.19600L;
const double GAUSSIAN_B    =  0.25472L;
const double GAUSSIAN_R1   =  0.27597L;
const double GAUSSIAN_R2   =  0.27846L;

const double PARABOLOIDE_R = (2.0 / 3.0);
const double PARABOLOIDE_C = 10.180339887498948482045868343656L; /* (5.0 * sqrt(5.0) - 1.0); */

/* limites para geração dos ensaios */
unsigned int NumFunc = 10000;
unsigned int NumPts  = 100;
unsigned int NumCos  = 20;
unsigned int NumSen  = 20;

#ifdef USE_SFMT
sfmt_t g_sfmt;
#endif

double runif(void)
{
#ifdef USE_SFMT
	//return genrand_res53();
	return sfmt_genrand_res53(&g_sfmt);
#else
	return rand() / (double)RAND_MAX;
#endif
}

void inicRNG(void)
{
#ifdef USE_SFMT
	//init_gen_rand((uint32_t)time(NULL));
	sfmt_init_gen_rand(&g_sfmt,(uint32_t)time(NULL));
#else
	srand((unsigned int)time(NULL));
#endif
}

double geraVarA(const double X, const double Y)
{
	return atan(Y * (2*X - 1));
}

double geraBernoulli(const double p)
{
	double u = runif();
	return (u < p) ? 1 : 0;
}

double geraCauchy(void)
{
	double u = runif();
	return tan(M_PI * (u - 0.5));
}

double geraExp(void)
{
	double u = runif();
	return (-2.0 * log(u));
}

double geraGeo(const double p)
{
	if (p == 1)
		return 1.0;
	double u = runif();
	return 1 + ceil(log(u) / (double)log(1 - p));
}

/* Ratio method (Kinderman-Monahan); see Knuth v2, 3rd ed, p130.
 * K+M, ACM Trans Math Software 3 (1977) 257-260.
 *
 * [Added by Charles Karney] This is an implementation of Leva's
 * modifications to the original K+M method; see:
 * J. L. Leva, ACM Trans Math Software 18 (1992) 449-453 and 454-455. */
double geraGaussian(void)
{
	double u, v, x, y, Q;

	/* This loop is executed 1.369 times on average  */
	do {
		/* Generate a point P = (u, v) uniform in a rectangle enclosing
		the K+M region v^2 <= - 4 u^2 log(u). */
		/* u in (0, 1] to avoid singularity at u = 0 */
		u = 1.0 - runif();
		/* v is in the asymmetric interval [-0.5, 0.5).  However v = -0.5
		is rejected in the last part of the while clause.  The
		resulting normal deviate is strictly symmetric about 0
		(provided that v is symmetric once v = -0.5 is excluded). */
		v = runif() - 0.5;
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

double geraRaioUnifParaboloide(void)
{
	double u = runif();
	double r = pow((1.0 + (u * PARABOLOIDE_C)), PARABOLOIDE_R);
	return(sqrt(r - 1.0) / 2.0);
}

double geraVarY_Ex(double t)
{
	/*
	Resolver EDO para obter Y
	y'' + L1 * y' + L2 * y = (E1 * t ^ 2) + (E2 * t) + E3 + Gamma(cos(t), sin(t), exp(t));
	y'' + L1 * y' + L2 * y = (E1 * t ^ 2) + (E2 * t) + E3 + GammaX * cos(t) + GammaY * sin(t) + GammaZ * exp(t);
	*/

	double L1 = geraExp();
	double L2 = 2 + geraGeo(L1 / (1 + L1));
	double E1 = geraGaussian();
	double E2 = E1 * geraGaussian();
	double E3_mean = (E1*E1) + (E2*E2);
	double E3_sigma = sqrt(pow(fabs(E1), 5.0) + (3.0 * (E1*E1) * pow(E2, 4.0)) + pow(fabs(E2), M_SQRT2));
	double E3 = E3_mean + E3_sigma * geraGaussian();
	double theta = M_2PI * runif();
	double radius = geraRaioUnifParaboloide();
	double GammaX = radius * cos(theta);
	double GammaY = radius * sin(theta);
	double GammaZ = radius * radius;
	double delta = (L1*L1) - (4.0 * L2);
	double Yp = 0.0; /* Solucao Particular; */
	double Yc = 0.0; /* Solucao Complementar (caso homogeneo) */
	double r1, r2, c1, c2;
	double YpA, YpB, YpC;
	double A, B;
	double sqrtDelta;

	/*
	Condicoes Iniciais:
	y (0) = 1;
	y'(0) = 1;
	*/

	if(delta > 0) {
		/*
		Yc  =       c1 * exp(r1 * t) +      c2 * exp(r2 * t);
		Yc' = r1 * c1 * exp(r1 * t) + r2 * c2 * exp(r2 * t);
		Yc (0) =      c1 +      c2 = 1 => c1 = (1 - c2);
		Yc'(0) = r1 * c1 + r2 * c2 = 1 => r1 * (1 - c2) + r2 * c2 = 1 => c2 = (1 - r1) / (r2 - r1);
		*/
		sqrtDelta = sqrt(delta);
		r1 = 0.5 * (-L1 + sqrtDelta);
		r2 = 0.5 * (-L1 - sqrtDelta);
		c2 = (1 - r1) / (r2 - r1);
		c1 = (1 - c2);
		Yc = c1 * exp(r1 * t) + c2 * exp(r2 * t);
	} else if(delta < 0) {
		/*
		Yc  = exp(A * t) * (c1 * cos(B * t) + c2 * sin(B * t));
		Y'c = exp(A * t) * ((A * c1 + B * c2) * cos(B * t) + (A * c2 - B * c1) * sin(B * t));
		Yc (0) = c1          = 1;
		Y'c(0) = A * c1 + B * c2 = 1 => c2 = (1 - A) / B;
		Y'c    = A * c1 + B * c2 = 1;
		A = -b / 2 * a = -b / 2;
		B = sqrt(4 * L2 - L1^2) / 2 * a;
		*/
		A = -0.5 * L1;
		B = 0.5 * sqrt(fabs(delta));
		c2 = (1 - A) / B;
		Yc = exp(A * t) * (cos(B * t) + c2 * sin(B * t));
	} else {
		/*
		Yc  =      c1 * exp(r * t) +     c2 * t * exp(r * t);
		Yc' =  r * c1 * exp(r * t) + r * c2 * t * exp(r * t);
		Yc (0) =     c1 + c2  = 1 => c2 = (1 - c1);
		Yc'(0) = r * c1       = 1 => c1 = (1 / r);
		*/
		r1 = -0.5 * L1;
		c1 = (1 / r1);
		c2 = (1 - c1);
		Yc = (c1 + c2 * t) * exp(r1 * t);
	}

	/* Solucao particular do caso nao homogeneo; */
	YpA = (E1 / L2);
	YpB = (E2 - 2 * L1 * YpA) / L2;
	YpC = ((GammaX * cos(t) + GammaY * sin(t) + GammaZ * exp(t)) - 2 * YpA - L1 * YpB + E3) / L2;
	Yp = YpA * (t*t) + YpB * t + YpC;

	return(Yp + Yc);
}

double geraVarY(double t)
{
	double Y;
	do {
		/* previde a utilizaçao de valores nao numericos (NaN); */
		Y = geraVarY_Ex(t);
	} while(Y != Y);
	return Y;
}

double geraFuncao(double* pValores, double* pCoefA, double* pCoefU)
{
	register unsigned int nIdx;
	const double  stepSeq = (1.0 / (double)NumPts);
	double* pVal;
	double* pAtuA;
	double* pAtuU;
	double  X, Y, x, fMax;
	double  base2;
	double  _tmp;

	/* Gera coeficientes de F */
	pAtuA = pCoefA;
	pAtuU = pCoefU;
	Y = geraVarY(geraCauchy());
	X = geraBernoulli(0.5);
	*pAtuA++ = geraVarA(X, Y); // a_0

	for(nIdx = 0; nIdx < NumCos; nIdx++) {
		Y = geraVarY(geraCauchy());
		X = geraBernoulli(0.5);
		*pAtuA++ = geraVarA(X, Y); // a_k
		*pAtuU++ = runif();  // u_k
	}

	for(nIdx = 0; nIdx < NumSen; nIdx++) {
		Y = geraVarY(geraCauchy());
		X = geraBernoulli(0.5);
		*pAtuA++ = geraVarA(X, Y); // b_w
		*pAtuU++ = runif(); // v_w
	}

	/* reseta */
	pAtuA = pCoefA;
	pAtuU = pCoefA;
	pVal = pValores;
	fMax = *pCoefA; // usa a_0 como primeiro maximo
	x = 0.0;

	while(x < 1.0) {
		pAtuA = pCoefA;
		pAtuU = pCoefU;
		*pVal = *pAtuA;
		base2 = 2.0;

		pAtuA++; // pula a_0

		for(nIdx = 1; nIdx <= NumCos; nIdx++) {
			_tmp = (*pAtuA * cos(M_2PI * nIdx * (x + *pAtuU))) / base2;
			*pVal += _tmp;
			base2 *= 2.0;
			pAtuA++;
			pAtuU++;
		}

		for(nIdx = 1; nIdx <= NumSen; nIdx++) {
			_tmp = (*pAtuA * sin(M_2PI * nIdx * (x*x + *pAtuU))) / pow(2.0, (nIdx + fabs(*pAtuA)));
			*pVal += _tmp;
			pAtuA++;
			pAtuU++;
		}

		if(fMax < *pVal)
			fMax = *pVal;

		pVal++;
		x += stepSeq;
	}

	return fMax;
}

void DumpArray2File(char* szArquivo, double* pValores, unsigned int nSize)
{
	FILE* fpArq = NULL;
	double* pVal;
	unsigned int i;

	fpArq = fopen(szArquivo, "w+b");
	if(fpArq != NULL) {
		i = 0;
		pVal = pValores;
		while(i++ < nSize) {
			fprintf(fpArq, "%.8f\n", *pVal);
			pVal++;
		}
		fclose(fpArq);
	}
}

void AppendArray2File(char* szArquivo, double* pValores, unsigned int nSize)
{
	char szSemaphore[255];
	double* pVal = pValores;
	unsigned int i = 0;

	sprintf(szSemaphore,"%s.lock",szArquivo);
	FILE* fpS = fopen(szSemaphore, "w+b");
	if(fpS == NULL)
		return;

	while(flock(fileno(fpS), LOCK_EX) != 0) {
		sleep(1);
	}

	FILE* fpArq = fopen(szArquivo, "a+b");
	if(fpArq != NULL) {
		fflush(fpArq);
		while(i++ < nSize) {
			fprintf(fpArq, "%.8f\n", *pVal++);
		}
		fflush(fpArq);
		fclose(fpArq);
	}

	flock(fileno(fpS), LOCK_UN);
	fclose(fpS);
	unlink(szSemaphore);
}

int main(int argc, char* argv[])
{
	long long timeStart = current_timestamp();
	unsigned int i = 0;
	double* pCoefA;
	double* pCoefU;
	double* pValores;
	double* pValMax;
	double* pMaxAtu;
	char    szArquivo[255];
	char    singleFile = 0;
	char    lastFuncPoints = 0;

	for(i = 1; i < (unsigned int)argc; i++) {
		if(*argv[i] == '-') {
			switch(toupper(*(argv[i]+1))) {
				case 'F': NumFunc = (unsigned int)atol(argv[i]+2); break;
				case 'P': NumPts  = (unsigned int)atol(argv[i]+2); break;
				case 'C': NumCos  = (unsigned int)atol(argv[i]+2); break;
				case 'S': NumSen  = (unsigned int)atol(argv[i]+2); break;
				case 'M': singleFile = 1; break;
				case 'L': lastFuncPoints = 1; break;
			}
		}
	}

	printf("NumFunc = %d; NumPts  = %d; NumCos  = %d; NumSen  = %d;\n",
		NumFunc, NumPts, NumCos, NumSen);

	if (NumFunc < 1 || NumPts < 10)
		return 0;

	inicRNG();

	pValores = calloc(NumPts, sizeof(double));
	pValMax  = calloc(NumFunc, sizeof(double));
	pCoefA   = calloc(1 + NumSen + NumCos, sizeof(double));
	pCoefU   = calloc(    NumSen + NumCos, sizeof(double));

	pMaxAtu = pValMax;
	for(i = 1; i <= NumFunc; i++) {
		for(;;) {
			*pMaxAtu = geraFuncao(pValores, pCoefA, pCoefU);
			if (*pMaxAtu != *pMaxAtu) {
				printf("NaN catched! Discarting...\n");
				continue;
			}
			break;
		}
		// if(i%1000==0) printf("F(%d)\n", i);
		pMaxAtu++;
	}

	if (lastFuncPoints) {
		sprintf(szArquivo, "F%dP%dPTS_%llu.TXT", NumFunc, NumPts, timeStart);
		DumpArray2File(szArquivo, pValores, NumPts);
	}

	if (singleFile) {
		sprintf(szArquivo, "%s.TXT", "MAX");
		AppendArray2File(szArquivo, pValMax, NumFunc);
	} else {
		sprintf(szArquivo, "F%dP%dMAX_%llu.TXT", NumFunc, NumPts, timeStart);
		DumpArray2File(szArquivo, pValMax, NumFunc);
	}

	free(pCoefU);
	free(pCoefA);
	free(pValMax);
	free(pValores);

	return 0;
}
