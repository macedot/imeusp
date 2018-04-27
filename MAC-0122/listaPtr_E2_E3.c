/*
  x = 0.333333
  y = 0.200000

 pd = &x;
*pd = 0.333333
 pd = 0x12FF78
&pd = 0x12FF6C

 pd = &y;
*pd = 0.200000
 pd = 0x12FF70
&pd = 0x12FF6C

*pd = (*pd) * x
*pd = 0.066667
 pd = 0x12FF70
&pd = 0x12FF6C
Press any key to continue
*/

int main(void) 
{
  double x,y, *pd;

  x = (1 / 3.0);
  y = (1 / 5.0);
  printf("  x = %f\n",x);
  printf("  y = %f\n",y);

  printf("\n");

  printf(" pd = &x;\n");
  pd = &x;
  printf("*pd = %f\n",*pd);
  printf(" pd = 0x%X\n",pd);
  printf("&pd = 0x%X\n",&pd);

  printf("\n");

  printf(" pd = &y;\n");
  pd = &y;
  printf("*pd = %f\n",*pd);
  printf(" pd = 0x%X\n",pd);
  printf("&pd = 0x%X\n",&pd);

  printf("\n");

  printf("*pd = (*pd) * x\n",*pd);
  *pd = (*pd) * x;
  printf("*pd = %f\n",*pd);
  printf(" pd = 0x%X\n",pd);
  printf("&pd = 0x%X\n",&pd);

  return(0);
}
