#include "mcTypeDef.h"
#include "CTime.h"

//-----------------------------------------------------------------------------
// uint32_t CTime::time(void)
//-----------------------------------------------------------------------------
uint32_t CTime::time(void)
{
#if defined(__GNUC__)
  return (uint32_t)(1000 * (clock() /  (double)CLOCKS_PER_SEC));
#else
  return (uint32_t)GetTickCount();
#endif
}
//-----------------------------------------------------------------------------
// uint32_t CTime::set(void)
//-----------------------------------------------------------------------------
uint32_t CTime::set(void)
{
  m_nlTime = time();
  return m_nlTime;
}
//-----------------------------------------------------------------------------
// uint32_t CTime::get(void)
//-----------------------------------------------------------------------------
uint32_t CTime::get(void)
{
  return (time() - m_nlTime);
}