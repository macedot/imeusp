////////////////////////////////////////////////////////////////////////////////
// Thiago Pinheiro de Macedo - NUSP 5124272
////////////////////////////////////////////////////////////////////////////////
// Renova o ambiente =o)
clear

function waitUser()
  printf("\n-> Digite `resume` para continuar ou `abort` para finalizar:");
  pause;
endfunction

////////////////////////////////////////////////////////////////////////////////
// VALIDA OS METODOS (original por Eduardo Oda)
// metodo  : funcao -> [x,y] = metodo(x0,xm,m,y0,f)
// testes  : int >= 1
// x0      : inicio do intervalo de validacao
// xm      : final do intervalo de validacao
// y0      : conticao inical
// y_exato : funcao solucao exata -> v = y_exato(x)
// f       : funcao do problema de cauchy -> v = f(x, y)
////////////////////////////////////////////////////////////////////////////////
function [y] = valida(metodo, testes, x0, xm, y0, y_exato, f)

  log_2 = log(2);
  m     = 10;
  h     = ((xm - x0) / m);

  y_xm  = y_exato(xm);
  [y]   = metodo(x0, xm, h, y0, f);
  yk    = y(m + 1, 1);

  e_k   = abs(yk - y_xm);

  printf("\n");

  printf("     m             h            e_h(E)          2^q             q      \n");
  printf("-----------+--------------+--------------+--------------+--------------\n");
  printf(" %9d   %1.10f   %1.10f        --             --      \n", m, h, e_k);

  for k = 1:max(testes, 2);
    h  = 0.5 * h;
    m  = ceil((xm - x0) / h);

    [y] = metodo(x0, xm, h, y0, f);

    e_k   = abs(y(m + 1, 1) - y_xm);
    ratio = abs(yk - y_xm) / e_k;
    q_k   = abs(log(ratio) / log_2);

    printf(" %9d   %1.10f   %1.10f   %1.10f   %1.10f\n", m, h, e_k, ratio, q_k);

    yk = y(m + 1, :);
  end

endfunction

////////////////////////////////////////////////////////////////////////////////
// (1) Considerando-se o PVI abaixo:
//     /
//    | y'(x) = 4 * x * sqrt(y(x))
//    | y (0) = 1
//    | t \in [0, 2]
//     \
// Solucao exata: y(x) = (1 + x^2)^2;
function v = lista08_ex1_f(x, y)
  v = 4 * x * sqrt(y);
endfunction

function v = lista08_ex1_y(x)
  v = [];
  i = 1;
  for t = x
    v(i) = (1 + t^2)^2;
    i = i + 1;
  end
endfunction
////////////////////////////////////////////////////////////////////////////////
// (a)
//  - Aplique o Metodo 1 ao PVI para obter uma aproximacao para y(2),
//    utilizando os passos de integracao:
//    h = 0.1
//    h = 0.05
//    h = 0.025
//  - Construa uma tabela contendo o erro absoluto entre as solucoes
//    aproximadas e analitica para cada passo de integracao;
//  - Analise os resultados obtidos;
//  - Use o valor da solucao analitica para obter os valores iniciais necessarios;
////////////////////////////////////////////////////////////////////////////////
function [y] = Metodo1(t0, tm, h, y0, f)
  h_23   = h * 2 / 3;
  t      = t0:h:tm;
  [n, m] = size(t)
  y      = [y0 ; lista08_ex1_y(t(2))];

  for k = 3:m
    y(k) = y(k-1) + h*f(t(k-1), y(k-1)) - h_23*f(t(k-2), y(k-2));
  end
endfunction

function procEx01_Metodo1()
  printf("\n\n(ooo) Validacao do Metodo 1:\n");
  [y1] = valida(Metodo1, 10, t0, tm, y0, lista08_ex1_y, lista08_ex1_f);

  printf("\n\n(1.a) Aplicacao do Metodo 1:\n");
  h = [0.1 ; 0.05 ; 0.025];
  for i=1:3
    printf("      h = %1.4f:\n", h(i));
    t = t0:h(i):tm;
    y_ext = lista08_ex1_y(t);
    y_num = Metodo1(t0, tm, h(i), y0, lista08_ex1_f);
    printf("\n");
    printf("     y(t_k)           y_k         abs(e_k)    \n");
    printf("+--------------+--------------+--------------+\n");
    [one,n] = size(t);
    for k = 1:n
      e_k = abs(y_ext(k) -  y_num(k));
      printf(" %2.10f   %2.10f   %2.10f\n", y_ext(k), y_num(k), e_k);
    end
    printf("\n");
    printf("\n");
    clf();
    plot(t, [y_ext y_num]);
    waitUser();
  end
endfunction;
////////////////////////////////////////////////////////////////////////////////
// (b)
// - Aplique o Metodo 2 ao PVI, para a = 0 e a = -5 e obtenha
//   uma aproximacao para y(2), utilizando os passos de integracao:
//   h = 0.1
//   h = 0.05
//   h = 0.025
// - Construa uma tabela contendo o erro absoluto entre as solucoes aproximadas
//   e analitica para cada passo de integracao.
// - Analise os resultados obtidos.
// - Use o valor da solucao analitica para obter os valores iniciais necessarios.
////////////////////////////////////////////////////////////////////////////////
function [y] = Metodo2(t0, tm, h, y0, f)
  h_2    = h / 2;
  t      = t0:h:tm;
  [n, m] = size(t)
  y      = [y0 ; lista08_ex1_y(t(2))];

  for k = 3:m
    y(k) = (1 + a)* y(k-1) - a*y(k-2) + h_2 * ( (3 - a)*f(t(k-1), y(k-1)) - (1 + a)*f(t(k-2), y(k-2)) );
  end
endfunction

function procEx01_Metodo2()
  a1 = [0 ; -5];
  h  = [0.1 ; 0.05 ; 0.025];

  printf("\n\n(ooo) Validacao do Metodo 2:\n");
  for j=1:2
    a = a1(j);
    printf("\n      a = %d:\n", a);
    [y2] = valida(Metodo2, 10, t0, tm, y0, lista08_ex1_y, lista08_ex1_f);
  end;

  printf("\n\n(1.b) Aplicacao do Metodo 2:\n");
  for j=1:2
    a = a1(j);
    printf("      a = %d:\n", a);
    for i=1:3
      printf("      h = %1.4f:\n", h(i));
      t = t0:h(i):tm;
      y_ext = lista08_ex1_y(t);
      y_num = Metodo2(t0, tm, h(i), y0, lista08_ex1_f);
      printf("\n");
      printf("+--------------+--------------+--------------+\n");
      printf("|    y(t_k)    |      y_k     |   abs(e_k)   |\n");
      printf("+--------------+--------------+--------------+\n");
      [one,n] = size(t);
      for k = 1:n
        e_k = abs(y_ext(k) -  y_num(k));
        printf(" %2.10f   %2.10f   %2.10f\n", y_ext(k), y_num(k), e_k);
      end
      printf("\n");
      printf("\n");
      clf();
      plot(t, [y_ext y_num]);
      waitUser();
    end
  end
endfunction;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
function procEx01()
  t0 = 0;
  tm = 2;
  y0 = [1];

  procEx01_Metodo1();
  procEx01_Metodo2();
endfunction;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// (2) Considerando-se o PVI abaixo:
//     /
//    | y'(x) = y - x
//    | y (0) = 2
//    | t \in [0, 1]
//     \
// Solucao exata: y(x) = exp(x) + x + 1;
function v = lista08_ex2_f(x, y)
  v = (y - x);
endfunction

function v = lista08_ex2_y(x)
  v = [];
  i = 1;
  for t = x
    v(i) = exp(t) + t + 1;
    i = i + 1;
  end
endfunction

////////////////////////////////////////////////////////////////////////////////
function procEx02()
  h  = 0.1;
  t0 = 0;
  tm = 1;
  t  = t0:h:tm;
  y  = [2];
  f  = lista08_ex2_f;
  [n,m] = size(t);

  // Metodo de Euler Aprimorado
  k  = 1;
  f_calc = f(t(k), y(k));
  y_tilde = y(k) + (h / 2) * ( f_calc + f( t(k+1), y(k) + h*f_calc ) )
  y  = [y ; y_tilde];

  for k = 2:(m-1)
    f_calc = f(t(k), y(k));

    // Metodo de Nystron
    y(k+1) = y(k-1) + 2 * h * f_calc;

    for i = 1:2
  //    y(k+1) = f(t(k+1), y(k+1));

      // Metodo Implicito do Trapezio
      y(k+1) = y(k) + (h / 2) * (f_calc + f(t(k+1), y(k+1)));
    end

//    y(k+1) = f(t(k+1), y(k+1));
  end

  y_ext = lista08_ex2_y(t);

  printf("\n\n(2) Aplicacao do Metodo Preditor-Corretor:\n\n");
  printf("     y(t_k)           y_k         abs(e_k)    \n");
  printf("+--------------+--------------+--------------+\n");
  for k = 1:m
    e_k = abs(y_ext(k) -  y(k));
    printf(" %2.10f   %2.10f   %2.10f\n", y_ext(k), y(k), e_k);
  end

  clf();
  plot(t, [y_ext y]);
endfunction;


////////////////////////////////////////////////////////////////////////////////
// MAIN =]
////////////////////////////////////////////////////////////////////////////////
//procEx01();
procEx02();
// [EoF]

