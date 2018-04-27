#ifndef CTIME_H
#define CTIME_H

/******************************************************************************
 * Definicoes internas
 ******************************************************************************/

/******************************************************************************
 * Includes da Classe;
 ******************************************************************************/
#include <time.h>

#if defined(WIN32)
#  include <windows.h>
#endif

/******************************************************************************
 * Classe CRand
 ******************************************************************************/
class CTime
{
  private :
    uint32_t  m_nlTime;

  public :
    CTime() { set(); }
    ~CTime() { /* Dummy */  }

  public :
    uint32_t  time(void);
    uint32_t  set (void);
    uint32_t  get (void);
};


#endif // CTIME_H