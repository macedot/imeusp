#ifndef CFILA_H
#define CFILA_H

/******************************************************************************
 * Includes da Classe;
 ******************************************************************************/
#include "CElem.h"

/******************************************************************************
 * Definicoes internas
 ******************************************************************************/
typedef struct listaElem {
  CElem*         pElem;
  struct listaElem*  pNext;
} ListaElem; 

typedef struct filaElem {
  ListaElem*  pLista;
  ListaElem*  pLast;
  uint32_t    nQuant[TIPO_ELEM_MAX];
  uint32_t    nTam;
} FilaElem; 

/******************************************************************************
 * Classe CFila
 ******************************************************************************/
class CFila
{
//  private :
  public :
    FilaElem   m_sPendentes;  // Elemntos na fila;
    FilaElem   m_sAtendidos;  // Elementos atendidos;

  public :
    CFila();
    ~CFila();
  
  public :
    void      Inclui       (TIPOELEM TipoElem, uint32_t nlTime);
    CElem*    PegaProximo  (BOOL bPref);
    CElem*    AtendeProximo(void);
    uint32_t  Pendentes    (void);
    uint32_t  Atendidos    (void);

  private:
    void AppendElem    (FilaElem* pFilaElem, ListaElem* pListaElem);
    void LiberaFilaElem(FilaElem* pFilaElem);
};


#endif // CFILA_H
