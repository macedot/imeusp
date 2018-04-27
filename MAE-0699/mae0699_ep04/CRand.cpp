#include "mcTypeDef.h"
#include "CRand.h"

#if USE_SFMT

extern "C" {
#  include "SFMT.h"
}

#endif

//-----------------------------------------------------------------------------
// void CRand::inic(void)
//-----------------------------------------------------------------------------
void CRand::inic(void)
{
#if USE_SFMT
  init_gen_rand((uint32_t)GetTickCount());
#else
  srand((uint32_t)clock());
#endif
}
//-----------------------------------------------------------------------------
// double CRand::randUniform(void)
//-----------------------------------------------------------------------------
double CRand::randUniform(void)
{
#if USE_SFMT
  return ((double)genrand_res53());
#else
  return (double)(rand() / (double)RAND_MAX);
#endif /* USE_SFMT */
}
//-----------------------------------------------------------------------------
// double CRand::randUniformPos(void)
//-----------------------------------------------------------------------------
double CRand::randUniformPos(void)
{
  double u;

  do {
    u = randUniform();
  } while (u == 0.0);
  return u;
}
//-----------------------------------------------------------------------------
// double CRand::randPoisson(const double fLambda)
// Como implementado na GSL: http://www.gnu.org/software/gsl/
//
// "The poisson distribution has the form
//   p(n) = (fLambda ^ n / n!) exp(-fLambda) 
//   for n = 0, 1, 2, ... . The method used here is the one from Knuth.
//-----------------------------------------------------------------------------
uint32_t CRand::randPoisson(const double fLambda)
{
  uint32_t  k = 0;
//   uint32_t  m;
  double  lambda = fLambda;
//  double  X;
  double  expLambda;
  double  prod;

/*
  while(lambda > 10)
  {
    m = lambda * (7.0 / 8.0);
    X = randGammaInt(m);
    if (X >= lambda)
    {
      return k + gsl_ran_binomial(r, (lambda / X), (m - 1));
    }
    k += m;
    lambda -= X; 
  }
*/
  /* This following method works well when lambda is small */
  expLambda = exp(-lambda);
  prod = 1.0;
  do {
    prod *= randUniform();
    k++;
  } while(prod > expLambda);

  return k - 1;
}
/****************************************************************************** 
 * Como implementado na GSL: http://www.gnu.org/software/gsl/
 *
 * "[Added by Charles Karney] We use Leva's implementation of the Ratio
 * method which avoids calling log() nearly all the time and makes the
 * Ratio method faster than the Polar method (when it produces just one
 * result per call).  Timing per call (gcc -O2 on 866MHz Pentium,
 * average over 10^8 calls)

 * Ratio method (Kinderman-Monahan); see Knuth v2, 3rd ed, p130.
 * K+M, ACM Trans Math Software 3 (1977) 257-260.
 *
 * [Added by Charles Karney] This is an implementation of Leva's
 * modifications to the original K+M method; see:
 * J. L. Leva, ACM Trans Math Software 18 (1992) 449-453 and 454-455. 
 *****************************************************************************/
//-----------------------------------------------------------------------------
// double CRand::randNormal(void)
//-----------------------------------------------------------------------------
double CRand::randNormal(void)
{
  static const double s  =  0.449871;    /* Constants from Leva */
  static const double t  = -0.386595;
  static const double a  =  0.19600;
  static const double b  =  0.25472;
  static const double r1 =  0.27597;
  static const double r2 =  0.27846;
  double u, v, x, y, Q;

  do /* This loop is executed 1.369 times on average  */
    {
      /* Generate a point P = (u, v) uniform in a rectangle enclosing
         the K+M region v^2 <= - 4 u^2 log(u). */

      /* u in (0, 1] to avoid singularity at u = 0 */
      u = 1 - randUniform();

      /* v is in the asymmetric interval [-0.5, 0.5).  However v = -0.5
         is rejected in the last part of the while clause.  The
         resulting normal deviate is strictly symmetric about 0
         (provided that v is symmetric once v = -0.5 is excluded). */
      v = randUniform() - 0.5;

      /* Constant 1.7156 > sqrt(8/e) (for accuracy); but not by too
         much (for efficiency). */
      v *= 1.7156;

      /* Compute Leva's quadratic form Q */
      x = u - s;
      y = fabs (v) - t;
      Q = x * x + y * (a * y - b * x);

      /* Accept P if Q < r1 (Leva) */
      /* Reject P if Q > r2 (Leva) */
      /* Accept if v^2 <= -4 u^2 log(u) (K+M) */
      /* This final test is executed 0.012 times on average. */
    }
  while (Q >= r1 && (Q > r2 || v * v > -4 * u * u * log (u)));

  return (v / u);       /* Return slope */
}
//-----------------------------------------------------------------------------
// double CRand::randNormal(const double fMedia, const double fDesvioPadrao)
//-----------------------------------------------------------------------------
double CRand::randNormal(const double fMedia, const double fDesvioPadrao)
{
  return (fMedia + fDesvioPadrao * randNormal());
}
/****************************************************************************** 
 * The lognormal distribution has the form 
 * 
 * p(x) dx = 1/(x * sqrt(2 pi sigma^2)) exp(-(ln(x) - zeta)^2/2 sigma^2) dx
 * 
 * for x > 0. Lognormal random numbers are the exponentials of
 * gaussian random numbers 
 *****************************************************************************/
//-----------------------------------------------------------------------------
// double CRand::randLogNormal(const double zeta, const double sigma)
//-----------------------------------------------------------------------------
double CRand::randLogNormal(const double zeta, const double sigma)
{
  double u, v, r2, normal, z;

  do {
    /* choose x,y in uniform square (-1,-1) to (+1,+1) */
    u = -1 + 2 * randUniform();
    v = -1 + 2 * randUniform();
    /* see if it is in the unit circle */
    r2 = u * u + v * v;
  } while (r2 > 1.0 || r2 == 0);
  normal = u * sqrt (-2.0 * log (r2) / r2);
  z =  exp (sigma * normal + zeta);
  return z;
}
/****************************************************************************** 
 * The Gamma distribution of order a>0 is defined by:
 * 
 * p(x) dx = {1 / \Gamma(a) b^a } x^{a-1} e^{-x/b} dx
 * 
 * for x>0.  If X and Y are independent gamma-distributed random
 * variables of order a1 and a2 with the same scale parameter b, 
 * then X+Y has gamma distribution of order a1+a2.
 * 
 * The algorithms below are from Knuth, vol 2, 2nd ed, p. 129. 
 *****************************************************************************/
//-----------------------------------------------------------------------------
// double CRand::randGamma(const double a, const double b)
//-----------------------------------------------------------------------------
double CRand::randGamma(const double a, const double b)
{
  /* assume a > 0 */
  uint32_t  na = (uint32_t)floor(a);

  if (a == na)
  {
    return b * randGammaInt(na);
  }
  if (na == 0)
  {
    return b * randGammaFrac(a);
  }
  return b * (randGammaInt(na) + randGammaFrac(a - na)) ;
}
//-----------------------------------------------------------------------------
// double CRand::randGammaInt(const uint32_t a)
//-----------------------------------------------------------------------------
double CRand::randGammaInt(const uint32_t a)
{
  if (a < 12)
  {
    double prod = 1.0;
    uint32_t i;

    for (i = 0; i < a; i++)
    {
      prod *= randUniformPos();
    }

    /* Note: for 12 iterations we are safe against underflow, since
    the smallest positive random number is O(2^-32). This means
    the smallest possible product is 2^(-12*32) = 10^-116 which
    is within the range of double precision. */

    return -log (prod);
  }
  return randGammaLarge((double) a);
}
/****************************************************************************** 
 * Works only if a > 1, and is most efficient if a is large
 * This algorithm, reported in Knuth, is attributed to Ahrens.  
 * A faster one, we are told, can be found in: 
 * J. H. Ahrens and U. Dieter, Computing 12 (1974) 223-246.  
 *****************************************************************************/
//-----------------------------------------------------------------------------
// double CRand::randGammaLarge (const double a)
//-----------------------------------------------------------------------------
double CRand::randGammaLarge (const double a)
{
  double sqa, x, y, v;

  sqa = sqrt (2 * a - 1);
  do {
    do {
      y = tan (M_PI * randUniform());
      x = sqa * y + a - 1;
    } while (x <= 0);
    v = randUniform();
  } while (v > (1 + y * y) * exp ((a - 1) * log (x / (a - 1)) - sqa * y));
  return x;
}
/****************************************************************************** 
 * This is exercise 16 from Knuth; see page 135, 
 * and the solution is on page 551.  
 *****************************************************************************/
//-----------------------------------------------------------------------------
// double CRand::randGammaFrac(const double a)
//-----------------------------------------------------------------------------
double CRand::randGammaFrac(const double a)
{

  double p, q, x, u, v;

  p = M_E / (a + M_E);
  do {
    u = randUniform();
    v = randUniformPos();
    if (u < p)
    {
      x = exp ((1 / a) * log (v));
      q = exp (-x);
    }
    else
    {
      x = 1 - log (v);
      q = exp ((a - 1) * log (x));
    }
  } while (randUniform() >= q);
  return x;
}
