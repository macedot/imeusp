#include <stdio.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <memory.h>

#define  EPSON   (1.0E-15)

char sign(const double X)
{
  if(X > DBL_EPSILON)
    return 1;
  if(X < (-1.0 * DBL_EPSILON))
    return -1;
  return 0;
}

double evalFunc(const double fValX, const double* pCoef, const int nGrade)
{
  double fEval;
  int i;

  fEval = pCoef[nGrade];
  for(i = (nGrade - 1); i >= 0; i--)
  {
    fEval = pCoef[i] + fValX * fEval;
  }
  return fEval;
}

double evalFuncD1(const double fValX, const double* pCoef, const int nGrade)
{
  double fEval;
  int i;

  fEval = nGrade * pCoef[nGrade];
  for(i = (nGrade - 1); i > 0; i--)
  {
    fEval = i * pCoef[i] + fValX * fEval;
  }
  return fEval;
}

double cauchyCriteria(const double* pCoef, const int nGrade)
{
  double fM, fAux;
  int  i;

  fM = 0.0;
  for(i = 0; i < nGrade; i++)
  {
    fAux = fabs(pCoef[i] / pCoef[nGrade]);
    if(fAux > fM) 
      fM = fAux;
  }
  return (1.0 + fM);
}

double newtonMethod(const double fIniX, const double* pCoef, const int nGrade)
{
  double  fXn, fXn2E, fF_Xn, fFD1_Xn;
  char    Sign[2];

  fXn = fIniX;

  do {
    fF_Xn   = evalFunc(fXn, pCoef, nGrade);
    fFD1_Xn = evalFuncD1(fXn, pCoef, nGrade);
    fXn    -= (fF_Xn / fFD1_Xn);
    Sign[0] = sign(fF_Xn);
    if(!Sign[0]) 
      return fXn;
    fXn2E   = fXn - Sign[0] * 2.0 * EPSON;
    Sign[1] = sign(evalFunc(fXn2E, pCoef, nGrade));
    if(!Sign[1]) 
      return fXn2E;
  } while(Sign[1] == Sign[0]);
  return (fXn + Sign[0] * EPSON);
}

double* findRoots(const double* fCoef,const int nParamGrade)
{
  double* pRoots;
  double  fM;
  int     nRoot;
  int     nGrade;
  int     i;

  pRoots = (double*)malloc((nParamGrade + 1) * sizeof(double));
  if(pRoots == NULL)
    return NULL;

  memcpy(pRoots, fCoef, sizeof(pRoots) * sizeof(double));
  for(nRoot = 0, nGrade = nParamGrade; (nGrade  > 1); nGrade-- , nRoot++)
  {
    /* Cauchy criteria to find root boudaries; */
    fM = - cauchyCriteria(pRoots+nRoot, nGrade);
    /* Newton Method to find de lesser root; (left root); */
    pRoots[nRoot] = newtonMethod(fM, pRoots+nRoot, nGrade);
    /* Horner method to divive the polynom coeficients; */
    for(i = (nGrade - 1); i > 0; i--)
    {
      pRoots[nRoot + i] += pRoots[nRoot] * pRoots[nRoot + i + 1];
    }
  }
  /* Root from an lienar polynom; */
  pRoots[nRoot] /= pRoots[i];
  return pRoots;
}

int main(void)
{
  double  fCoef[4] = { 10.0, 3.0, -6.0, 1.0 };
  double* pRoots;
  int     i;
  int     nSize  = (sizeof(fCoef) / sizeof(double));
  int     nGrade = (nSize - 1);

  pRoots = findRoots(fCoef, nGrade);
  if(pRoots != NULL)
  {
    printf("+---+--------------+-----------------+\n");
    printf("| n |      Xn      |      f(Xn)      |\n");
    printf("+---+--------------+-----------------+\n");
    for(i = 0; i < nGrade; i++)
    {
      printf("|%3d|%14.10g|%17.10g|\n", i+1, pRoots[i], evalFunc(pRoots[i], fCoef, nGrade));
    }
    printf("+---+--------------+-----------------+\n");
    free(pRoots);
    pRoots = NULL;
  }
	return 0;
}
/*
+---+--------------+-----------------+
| n |      Xn      |      f(Xn)      |
+---+--------------+-----------------+
|  1|            -1|-1.953992523e-014|
|  2|             2|-2.486899575e-014|
|  3|             5|-1.278976924e-013|
+---+--------------+-----------------+
*/