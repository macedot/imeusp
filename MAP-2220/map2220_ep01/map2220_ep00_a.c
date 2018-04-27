#include <stdio.h>

double funcao_F(double X)
{
  double Y;
  Y = X * X * (X - 2) - 5;
  return Y;
}

double funcao_F1(double X)
{
  double Y;
  Y = (3 * X - 4) * X;
  return Y;
}

char sign(double X)
{
  return (X > 0.0 ? '+' : (X < 0.0 ? '-' : ' '));
}

int main(void)
{
  double Xn, Xn_2E, FXn, F1Xn, Epson;
  unsigned int n;
  char  Sign[2];

  n = 0;
  Epson = 1.0E-6;
  printf("E = %G = %.6f;\n", Epson, Epson);
  printf("s : sign[F(Xn   )];\n");
  printf("S : sign[F(Xn-2E)];\n");
  printf("+---+--------------+---+--------------+---+\n");
  printf("| n |      Xn      | s |     Xn-2E    | S |\n");
  printf("+---+--------------+---+--------------+---+\n");
  Xn      = 4.0;
  Xn_2E   = Xn - 2.0 * Epson;
  FXn     = funcao_F(Xn);
  F1Xn    = funcao_F1(Xn);
  Sign[0] = sign(FXn);
  Sign[1] = sign(funcao_F(Xn_2E));
  printf("|%3d| %.10f | %c | %.10f | %c |\n", n, Xn, Sign[0], Xn_2E, Sign[1]);
  for(n = 1; ;n++)
  {
    FXn     = funcao_F(Xn);
    F1Xn    = funcao_F1(Xn);
    Xn      = Xn - (FXn / F1Xn);
    Xn_2E   = Xn - 2.0 * Epson;
    Sign[0] = sign(FXn);
    Sign[1] = sign(funcao_F(Xn_2E));
    printf("|%3d| %.10f | %c | %.10f | %c |\n", n, Xn, Sign[0], Xn_2E, Sign[1]);
    if(Sign[1] != '+') break;
    Xn = Xn_2E;
  }
  printf("+---+--------------+---+--------------+---+\n");
  printf("X ~ (Xn - Epson) = %.10f\n", Xn - Epson);
	return 0;
}
/*
E = 1E-006 = 0.000001;
s : sign[F(Xn   )];
S : sign[F(Xn-2E)];
+---+--------------+---+--------------+---+
| n |      Xn      | s |     Xn-2E    | S |
+---+--------------+---+--------------+---+
|  0| 4.0000000000 | + | 3.9999980000 | + |
|  1| 3.1562500000 | + | 3.1562480000 | + |
|  2| 2.7786025995 | + | 2.7786005995 | + |
|  3| 2.6946592341 | + | 2.6946572341 | + |
|  4| 2.6906563310 | + | 2.6906543310 | + |
|  5| 2.6906474481 | + | 2.6906454481 | - |
+---+--------------+---+--------------+---+
X ~ (Xn - Epson) = 2.6906464481
*/