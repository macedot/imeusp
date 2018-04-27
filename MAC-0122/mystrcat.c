/************************************************/
#include <stdio.h>

char* mystrcpy(char* szDestino, char* szOrigem) 
{
  char* pDest=szDestino;
  char* pOrig=szOrigem;
  
  while(*pOrig!='\0') 
  {
    *pDest=*pOrig;
    pDest++;
    pOrig++;
  }
  *pDest='\0';
  return(szDestino);
}

char* mystrcat(char* szDestino, char* szConcat) 
{
  char* pStr=szDestino;
  
  while(*pStr!='\0')
  {
    pStr++;
  }
  mystrcpy(pStr,szConcat);
  return(szDestino);
}

int main(void) 
{
  char szBuffer1[10];
  char szBuffer2[10];
  char szBuffer3[50];

  mystrcpy(szBuffer1,"TESTE1");
  mystrcpy(szBuffer2,"TESTE2");
  mystrcpy(szBuffer3,szBuffer1);
  mystrcat(szBuffer3,szBuffer2);

  printf("%s\n%s\n%s\n", szBuffer1,szBuffer2,szBuffer3);

  return(0);
}
/************************************************/
