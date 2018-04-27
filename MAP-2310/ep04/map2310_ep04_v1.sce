////////////////////////////////////////////////////////////////////////////////
// Thiago Pinheiro de Macedo - NUSP 5124272
// Resolucao dos Exerciciso apresentados na Lista #04
////////////////////////////////////////////////////////////////////////////////

// Renova o ambiente =o)
clear

////////////////////////////////////////////////////////////////////////////////
// VALIDA OS METODOS (original por Eduardo Oda)
//
// metodo  : funcao -> [x,y] = metodo(x0,xm,m,y0,f)
// testes  : int >= 1
// x0      : inicio do intervalo de validacao
// xm      : final do intervalo de validacao
// y0      : conticao inical
// y_exato : funcao solucao exata -> v = y_exato(x)
// f       : funcao do problema de cauchy -> v = f(x, y)
//
// Exemplo de Uso :
// valida(rkf   , 7, 0, 1, [-1,2], y_EX12, f_EX12)
// valida(rk44  ,10, 0, 1, [-1,2], y_EX12, f_EX12)
// valida(eulerA, 9, 0, 1, [-1,2], y_EX12, f_EX12)
// valida(euler ,11, 0, 1, [-1,2], y_EX12, f_EX12)
////////////////////////////////////////////////////////////////////////////////
function [x, y] = valida(metodo, testes, x0, xm, y0, y_exato, f)
  log_2 = log(2);
  m     = 10;

  y_xm  = y_exato(xm);
  [x,y] = metodo(x0, xm, m, y0, f);
  yk    = y(m + 1, 1);

  h     = ((x_m - x_0) / m);
  e_k   = abs(yk - y_xm);

  printf("     m     |       h      |     e_h(E)   |      2^q     |       q      \n");
  printf("-----------+--------------+--------------+--------------+--------------\n");
  printf(" %9d | %1.10f | %1.10f |      --      |      --      \n", m, h, e_k);

  for k = 1:max(testes, 2);
    h  = 0.5 * h;
    m  = 10.0 * 2^k

    [x, y] = metodo(x0, xm, m, y0, f);

    e_k   = abs(y(m + 1, 1) - y_xm);
    ratio = abs(yk - y_xm) / e_k;
    q_k   = log(ratio) / log_2;

    printf(" %9d | %1.10f | %1.10f | %1.10f | %1.10f\n", m, h, e_k, ratio, q_k);

    yk = y(m + 1, 1);
  end
endfunction

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
function waitUser()
  printf("\n-> Digite `resume` para continuar ou `abort` para finalizar:");
  pause;
endfunction

/////////////////////////////////////////////////////////////////////////////
// Metodo de Euler
/////////////////////////////////////////////////////////////////////////////
function [t,y] = metodoEuler(t0, tm, m, y0, f)
  h = (tm - t0) / m;
  t = t0;
  y = y0;
  for k = 1:m
    k1 = h * f(t(k), y(k,:));
    y  = [y; y(k,:) + k1];
    t  = [t; t(k)   + h];
  end
endfunction

////////////////////////////////////////////////////////////////////////////////
// Exercicio #01
//
// Problema de Cauchy:
// /
// | y(0)   = 1
// | y_x(x) = -y*tan(x) - 1/cos(x)
// \
//
// cuja solucao geral e:
// y(x) = cos(x) - sin(x)

function v = f_EX1(x, y)
  v = -y*tan(x) - 1/cos(x);
endfunction

function v = y_EX1(x)
  v = []
  i = 1;
  for s = x
    v(i) = cos(s) - sin(s);
    i = i + 1;
  end
endfunction

// (a)
// Problema-modelo para validacao do metodo de Euler:
// -> Equacao Logistica / Modelo de Verhust:
//    /
//   | y(x_0) = y_0
//   | dy/dx  = r * y(x)*( K - y(x) ) / K
//    \
// -> Solucao Geral:
//    y(x) = (K * exp(r*x + K*C)) / (exp(r*x + K*C) - 1)
// -> Caso particular para validarmos o modelo:
//    /
//   | r     = 1;
//   | K     = 42;
//   | y(0)  = 1
//   | dy/dx = y(x)*(42 - y(x)) /42
//    \
// -> Solucao:
//    y(x) = 42*exp(x) / (41 + exp(x))
function v = f_EX1A(x, y)
  v = y*(42 - y) / 42;
endfunction

function v = y_EX1A(x)
  v = []
  i = 1;
  for s = x
    v(i) = 42*exp(s) / (41 + exp(s));
    i = i + 1;
  end
endfunction

k_max = 10;

printf("\n(a)\n");
x_0   =  0;
x_m   = 10;
y_0   =  1;
[x, a] = valida(metodoEuler, k_max, x_0, x_m, y_0, y_EX1A, f_EX1A);
printf("\n\n");

// (1.b.1)
// Verificacao numerica do erro global de discretizacao;
printf("\n(1.b.1)\n");
x_0 = 0;
x_m = 1.292695719373;
y_0 = 1;
[x1, b1] = valida(metodoEuler, k_max, x_0, x_m, y_0, y_EX1, f_EX1);
printf("\n\n");
printf("-> Neste caso, a ordem de convergencia do metodo eh 4, possivelmente devida a escolha de um ponto especial\n");
printf("   onde ocorra algum cancelamento (numérico ou analítico) ou mesmo uma aproximacao os calculos que leve\n");
printf("   a despresarmos os terms dos erros.");
printf("\n\n");
// (1.b.2)
// Verificacao numerica do erro global de discretizacao;
printf("\n(1.b.2)\n");
x_0 = 0;
x_m = 1;
y_0 = 1;
[x2, b2] = valida(metodoEuler, k_max, x_0, x_m, y_0, y_EX1, f_EX1);
printf("\n\n");
printf("-> Neste caso, o metodo se comporta como esperado, tendo ordem de convergencia igual a 2.");
printf("\n\n");

// (1.c)
// Graficos dos dados obtidos nos items acima;
clf();
// Grafico de (1.b.1)
plot2d(x1,         b1, style=1);
plot2d(x1, y_EX1(x1'), style=3);
waitUser();
clf();
// Grafico de (1.b.2)
plot2d(x2,         b2, style=1);
plot2d(x2, y_EX1(x2'), style=3);
waitUser();

////////////////////////////////////////////////////////////////////////////////
// Exercicio #02
//
// Problema de Cauchy:
// /
// | y(0)   = 1
// | y_x(x) = -y*x / (1 - x^2)
// \
//
// cuja solucao geral e:
// y(x) = sqrt(1 - x^2)

function v = f_EX2(x, y)
  v = -y*x / (1 - x^2);
endfunction

function v = y_EX2(x)
  v = []
  i = 1;
  for s = x
    v(i) = sqrt(1 - s^2)
    i = i + 1;
  end
endfunction

// (2.b.1)
// Verificacao numerica do erro global de discretizacao;
printf("\n(2.b.1)\n");
x_0 = 0;
x_m = 0.5;
y_0 = 1;
[x1, b1] = valida(metodoEuler, k_max, x_0, x_m, y_0, y_EX2, f_EX2);
printf("\n\n");
printf("-> Neste caso, o metodo se comporta como esperado, tendo ordem de convergencia igual a 2.");
printf("\n\n");

// (2.b.2)
// Verificacao numerica do erro global de discretizacao;
printf("\n(2.b.2)\n");
x_0 = 0;
x_m = 1;
y_0 = 1;
[x2, b2] = valida(metodoEuler, k_max, x_0, x_m, y_0, y_EX2, f_EX2);
printf("\n\n");
printf("-> Neste caso, novamente devido a escolha de um ponto especial, o metodo tem ordem de convergencia diferente a teorica\n");
printf("   (ordem 0.5 neste caso), mas desta vez o erro nao eh cancelado e a convergencia eh prejudicada.");
printf("\n\n");

// (2.c)
// Graficos dos dados obtidos nos items acima;
clf();
// Grafico de (2.b.1)
plot2d(x1,         b1, style=1);
plot2d(x1, y_EX2(x1'), style=3);
waitUser();
clf();
// Grafico de (2.b.2)
plot2d(x2,         b2, style=1);
plot2d(x2, y_EX2(x2'), style=3);

////////////////////////////////////////////////////////////////////////////////
// [EoF]
////////////////////////////////////////////////////////////////////////////////
