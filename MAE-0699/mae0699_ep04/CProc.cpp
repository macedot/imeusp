//-----------------------------------------------------------------------------
// Prototipode Funcoes Externas
//-----------------------------------------------------------------------------
#include "mcTypeDef.h"
#include "CProc.h"
#include "CRand.h"


extern CRand  cRand;

//*****************************************************************************
// Classe CElem
//*****************************************************************************
//-----------------------------------------------------------------------------
// CProc::CProc();
//-----------------------------------------------------------------------------
CProc::CProc()
{
  m_nlTotOcioso   = 0;
  m_nlTotAtende   = 0;
  m_nlTempoAtende = 0;
  m_pElemento = NULL;
}
//-----------------------------------------------------------------------------
// CProc::~CProc();
//-----------------------------------------------------------------------------
CProc::~CProc()
{
}
//-----------------------------------------------------------------------------
// BOOL CProc::Ocupado(void);
//-----------------------------------------------------------------------------
BOOL CProc::Ocupado(void)
{
  return (m_pElemento != NULL);
}
//-----------------------------------------------------------------------------
// BOOL CProc::Atende(CElem* pElemento, uint32_t nlTime);
//-----------------------------------------------------------------------------
BOOL CProc::Atende(CElem* pElemento, uint32_t nlTime)
{
  if(Ocupado() || (m_pElemento == NULL))
    return FALSE;

  m_pElemento = pElemento;
  switch(m_pElemento->m_Tipo)
  {
    case TIPO_ELEM_PREF :
      m_nlTempoAtende = (uint32_t)ceil(cRand.randLogNormal(5.0, M_SQRT2));
      if(m_nlTempoAtende > 11) 
      {
        m_nlTempoAtende = 11;
      }
      break;
    case TIPO_ELEM_COMUM :
    default:
      m_nlTempoAtende = (uint32_t)ceil(cRand.randLogNormal(3.0, 1.0));
      if(m_nlTempoAtende > 6)  
      {
        m_nlTempoAtende = 6;
      }
      break;
  }
  m_pElemento->Atende(nlTime);
  return TRUE;
}
//-----------------------------------------------------------------------------
// void CProc::Atualiza(uint32_t nlTime);
//-----------------------------------------------------------------------------
void CProc::Atualiza(uint32_t nlTime)
{
  if(Ocupado())
  {
    if((nlTime - m_pElemento->m_nAtende) >= m_nlTempoAtende)
    {
      m_pElemento->Finaliza(nlTime);
    }
  }
  else
  {
    m_nlTotOcioso++;
  }

}
