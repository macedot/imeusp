#ifndef CELEMENTO_H
#define CELEMENTO_H

/******************************************************************************
 * Definicoes internas
 ******************************************************************************/
enum TIPOELEM {
  TIPO_ELEM_PREF = 0,
  TIPO_ELEM_COMUM,
  TIPO_ELEM_MAX
};

/******************************************************************************
 * Includes da Classe;
 ******************************************************************************/

/******************************************************************************
 * Classe CElem
 ******************************************************************************/
class CElem
{
  friend class CFila;

//  protected :
  public :
    TIPOELEM    m_Tipo;
    uint32_t    m_nEspera;
    uint32_t    m_nAtende;

  public :
    CElem(TIPOELEM Tipo, uint32_t nlTime);
    ~CElem();

  public :
    void Atende  (uint32_t nlTime);
    void Finaliza(uint32_t nlTime);


};


#endif // CELEMENTO_H
