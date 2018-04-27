//-----------------------------------------------------------------------------
// Prototipode Funcoes Externas
//-----------------------------------------------------------------------------
#include "mcTypeDef.h"
#include "CFila.h"

//-----------------------------------------------------------------------------
// Prototipode Funcoes Internas
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variaveis Globais Externas
//-----------------------------------------------------------------------------

//*****************************************************************************
// Classe CFila
//*****************************************************************************
//-----------------------------------------------------------------------------
// CFila::CFila(void)
//-----------------------------------------------------------------------------
CFila::CFila(void)
{
  memset(&m_sPendentes, 0, sizeof(FilaElem));
  memset(&m_sAtendidos, 0, sizeof(FilaElem));
}
//-----------------------------------------------------------------------------
// CFila::~CFila(void)
//-----------------------------------------------------------------------------
CFila::~CFila(void)
{
  if(m_sPendentes.nTam > 0)
  {
    LiberaFilaElem(&m_sPendentes);
  }
  if(m_sAtendidos.nTam > 0)
  {
    LiberaFilaElem(&m_sAtendidos);
  }
  memset(&m_sPendentes, 0, sizeof(FilaElem));
  memset(&m_sAtendidos, 0, sizeof(FilaElem));
}
//-----------------------------------------------------------------------------
// uint32_t CFila::Pendentes(void)
//-----------------------------------------------------------------------------
uint32_t CFila::Pendentes(void)
{
  return m_sPendentes.nTam;
}
//-----------------------------------------------------------------------------
// uint32_t CFila::Atendidos(void)
//-----------------------------------------------------------------------------
uint32_t CFila::Atendidos(void)
{
  return m_sAtendidos.nTam;
}
//-----------------------------------------------------------------------------
// void CFila::Inclui(TIPOELEM TipoElem, uint32_t nlTime)
//-----------------------------------------------------------------------------
void CFila::Inclui(TIPOELEM TipoElem, uint32_t nlTime)
{
  ListaElem*  pListaElem;

  pListaElem = new ListaElem;
  if(pListaElem == NULL)
  {
    return;
  }
  pListaElem->pElem = new CElem(TipoElem, nlTime);
  if(pListaElem->pElem == NULL)
  {
    delete pListaElem;
    return;
  }
  AppendElem(&m_sPendentes, pListaElem);
}
//-----------------------------------------------------------------------------
// CElem* CFila::PegaProximo(BOOL bPref)
//-----------------------------------------------------------------------------
CElem* CFila::PegaProximo(BOOL bPref)
{
  CElem*  pElem      = NULL;
  ListaElem*  pListaElem = NULL;

  if(m_sPendentes.nTam > 0)
  {
    pElem = m_sPendentes.pLista->pElem;
    if(bPref && (pElem->m_Tipo != TIPO_ELEM_PREF) && 
       (m_sPendentes.nQuant[TIPO_ELEM_PREF] > 0))
    {
      pListaElem = m_sPendentes.pLista;
      do {
        if(pListaElem->pElem->m_Tipo == TIPO_ELEM_PREF)
        {
          pElem = m_sPendentes.pLista->pElem;
          break; // do-while()
        }
        pListaElem = pListaElem->pNext;
      } while(pListaElem != NULL);
    }
  }
  return pElem;
}
//-----------------------------------------------------------------------------
// CElem* CFila::AtendeProximo(void)
//-----------------------------------------------------------------------------
CElem* CFila::AtendeProximo(void)
{
  ListaElem*  pListaElem = NULL;
  CElem*  pElem      = NULL;

  if(m_sPendentes.nTam > 0)
  {
    pElem = m_sPendentes.pLista->pElem;

    pListaElem = m_sPendentes.pLista;
    m_sPendentes.pLista = pListaElem->pNext;
    m_sPendentes.nQuant[pElem->m_Tipo]--;
    m_sPendentes.nTam--;

    AppendElem(&m_sAtendidos, pListaElem);
  }
  return pElem;
}
//-----------------------------------------------------------------------------
// void CFila::AppendElem(FilaElem* pFilaElem, ListaElem* pListaElem)
//-----------------------------------------------------------------------------
void CFila::AppendElem(FilaElem* pFilaElem, ListaElem* pListaElem)
{
  pListaElem->pNext = NULL;
  if(pFilaElem->pLista == NULL)
  {
    pFilaElem->pLast = pFilaElem->pLista = pListaElem;
  }
  else
  {
    pFilaElem->pLast->pNext = pListaElem;
    pFilaElem->pLast = pFilaElem->pLast->pNext;
  }
  pFilaElem->nQuant[pListaElem->pElem->m_Tipo]++;
  pFilaElem->nTam++;
}
//-----------------------------------------------------------------------------
// void CFila::LiberaFilaElem(FilaElem* pFilaElem)
//-----------------------------------------------------------------------------
void CFila::LiberaFilaElem(FilaElem* pFilaElem)
{
  ListaElem*  pNext;

  while(pFilaElem->pLista != NULL)
  {
    pNext = pFilaElem->pLista->pNext;
    delete pFilaElem->pLista;
    pFilaElem->pLista = pNext;
    pFilaElem->nTam--;
  } 
}

