#ifndef CPROCESSADOR_H
#define CPROCESSADOR_H

/******************************************************************************
 * Definicoes internas
 ******************************************************************************/
enum TIPOPROC {
  TIPO_PROC_PREF = 0,
  TIPO_PROC_COMUM,
  TIPO_PROC_MAX
};

/******************************************************************************
 * Includes da Classe;
 ******************************************************************************/
#include "CElem.h"

/******************************************************************************
 * Classe CProc
 ******************************************************************************/
class CProc
{
  private :
    uint32_t  m_nlTotOcioso;
    uint32_t  m_nlTotAtende;
    uint32_t  m_nlTempoAtende;
    CElem*    m_pElemento; // Elemento em Atendimento;
    

  public :
    CProc();
    ~CProc();

  public :
    BOOL  Ocupado (void);
    BOOL  Atende  (CElem* pElemento, uint32_t nlTime);
    void  Atualiza(uint32_t nlTime);


};


#endif // CPROCESSADOR_H
