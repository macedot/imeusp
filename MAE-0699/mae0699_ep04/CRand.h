#ifndef CRAND_H
#define CRAND_H

/******************************************************************************
 * Definicoes internas
 ******************************************************************************/
#define  USE_SFMT      (1) /* Utiliza SFMT para geracao de numeros aleatorios */

/******************************************************************************
 * Includes da Classe;
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
#include <math.h>
#ifdef __cplusplus
}
#endif


#if defined(WIN32)
#  include <windows.h>
#endif

/******************************************************************************
 * Classe CRand
 ******************************************************************************/
class CRand
{
  public :
      CRand() { inic();     };
     ~CRand() { /* dummy */ };

  public :
    void      inic          (void);
    double    randUniform   (void);
    double    randUniformPos(void);
    uint32_t  randPoisson   (const double fLambda);
    double    randNormal    (void);
    double    randNormal    (const double fMedia, const double fDesvioPadrao);
    double    randLogNormal (const double zeta, const double sigma);
    double    randGamma     (const double a, const double b);
    double    randGammaInt  (const unsigned int a);
    double    randGammaLarge(const double a);
    double    randGammaFrac (const double a);

};

#endif // CRAND_H
