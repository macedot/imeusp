#include <stdio.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <memory.h>

#ifndef M_PI
#  define M_PI       3.14159265358979323846
#endif

double funcao_U_Exata(double t, double x)
{
  double y;
  double x2;

  x2 = (x * x);
  y = 2.0 + sin(M_PI * (x - t)) - (t*x*x2) / 3.0 + ((x2*x2) - pow((x - t), 4.0)) / 12.0;
  return y;
}

double* Lax_Wendroff(double* pX, int M, int N, double k, double h)
{
  double*  pCalc;
  double   s, s2, k2;
  double   x, t;
  int      i, j, nTam;

  nTam = (N + 2*M);
  pCalc = (double*)malloc((nTam + 1) * sizeof(double));
  if(pCalc != NULL)
  {
    s  = (k / h);
    s2 = (s * s);
    k2 = (k * k);

    s  *= 0.5;
    s2 *= 0.5;
    k2 *= 0.5;

    t  = 0.0;
    for(i = (M - 1); i >= 0 ; i--)
    {
      x = -(h*i);
      for(j = 1; j < nTam; j++)
      {
        pCalc[j-1] = pX[j] -  s * (pX[j+1] - pX[j-1]) + s2 * (pX[j-1] - 2.0*pX[j] + pX[j+1])
                     /* parte nao-homogenea */
                     + x*(k2*(2*t - x) - k*x*t);
                     
        x += h;
      }
      nTam -= 2;
      t    += k;
      memcpy(pX, pCalc, (nTam + 1) * sizeof(double));
    }
    free(pCalc);
    pCalc = NULL;
  }
  return pX;
}

int main(void)
{
  int     i;
  int     M;
  int     N;
  int     nTam;
  double* pX;
  double* pNumeric;
  double* pExact;
  double  h;
  double  k;
  double  x;

  k = 0.05; /* t */
  h = 0.1;  /* x */
  M = (int)(1 / k); /* t */
  N = (int)(1 / h); /* x */
  /* 
    Assegura que tenhamos o numero "correto" de etapas; 
    Temos que fazer isso, pois o arredondamento no Linux nao foi o esperado;
  */
  while((M % 10)!=0) M++;
  while((N % 10)!=0) N++;
  nTam   = (N + 2*M);
  pX     = (double*)malloc((nTam + 1) * sizeof(double));
  pExact = (double*)malloc((N    + 1) * sizeof(double));

  /* Inicializa com a solucao em t = 0; */
  x = - (h * M);
  for(i = 0; i <= nTam; i++)
  { 
    pX[i] = 2.0 + sin(M_PI * x); /* funcao F(x) */
    x += h;
  }
  /* Aplica Lax-Wendroff; */
  pNumeric = Lax_Wendroff(pX, M, N, k, h);
  /* calcula posicao final na solucao "exata"; */
  for(i = 0; i <= N; i++)
  {
    pExact[i] = funcao_U_Exata(1, i*h);
  }
  /* Imprime resultados; */
  printf("-------+--------------+--------------+---------------\n");
  printf("   x   | Lax-Wendroff | Sol. 'Exata' | Erro Relativo \n");
  printf("-------+--------------+--------------+---------------\n");
  for(i = 0; i <= N; i++)
  {
    printf("  %.1f  | %1.10f | %1.10f | %+1.10f \n", 
           i*h, pNumeric[i], pExact[i], (pNumeric[i] - pExact[i]) / pExact[i]);
  }
  printf("-------+--------------+--------------+---------------\n");
  free(pExact);
  free(pX);
  return 0;
}
/*
-------+--------------+--------------+---------------
   x   | Lax-Wendroff | Sol. 'Exata' | Erro Relativo
-------+--------------+--------------+---------------
  0.0  | 1.8793588384 | 1.9166666667 | -0.0194649539
  0.1  | 1.6020677184 | 1.6359830056 | -0.0207308310
  0.2  | 1.3484155557 | 1.3755480810 | -0.0197248832
  0.3  | 1.1450180285 | 1.1626496723 | -0.0151650529
  0.4  | 1.0125926714 | 1.0189434837 | -0.0062327425
  0.5  | 0.9639308987 | 0.9583333333 | +0.0058409378
  0.6  | 1.0026458921 | 0.9856101504 | +0.0172844625
  0.7  | 1.1228189165 | 1.0959830056 | +0.0244856998
  0.8  | 1.3095786880 | 1.2755480810 | +0.0266792036
  0.9  | 1.5405570779 | 1.5026496723 | +0.0252270415
  1.0  | 1.7880786616 | 1.7500000000 | +0.0217592352
-------+--------------+--------------+---------------
*/