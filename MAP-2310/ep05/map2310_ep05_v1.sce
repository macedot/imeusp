////////////////////////////////////////////////////////////////////////////////
// Thiago Pinheiro de Macedo - NUSP 5124272
////////////////////////////////////////////////////////////////////////////////

// Renova o ambiente =o)
clear

auto_step = 1;

////////////////////////////////////////////////////////////////////////////////
// Metodo "MidPoint"
////////////////////////////////////////////////////////////////////////////////
function [t, y, hs] = MetodoMidPoint(t0, tm, m, y0, f)
  h   = (tm - t0) / m;
  h_2 = 0.5 * h;
  t   = t0;
  y   = y0;
  hs  = [h];
  
  for k = 1:m
    k1  = f(t(k)       , y(k,:)           );
    k2  = f(t(k) + h_2 , y(k,:) + h_2 * k1);
    
    y   = [y ; y(k,:) + h *k2];
    t   = [t ; t(k)   + h    ];
  end
endfunction
////////////////////////////////////////////////////////////////////////////////
// METODO DE RUNGE-KUTTA-FEHLBERG
//
// Para validar este metodo comente o ajuste de passo e escolha a y referente
// ao Runge-Kutta que deseja validar.
////////////////////////////////////////////////////////////////////////////////
function [x, y, hs] = rkf(x0, xm, m, y0, f)
  h  = (xm - x0) / m;
  x  = x0;
  y  = y0;
  k  = 1;
  hs = [h];
  
  minError = 10^(-10);
  maxError = 10^(-6);
  
  while x(k) < xm
    k1 = f( x(k)        , y(k,:)                                                                                  );
    k2 = f( x(k)+h/4    , y(k,:) + h*k1/4                                                                         );
    k3 = f( x(k)+h*3/8  , y(k,:) + h*k1*3/32      + h*k2*9/32                                                     );
    k4 = f( x(k)+h*12/13, y(k,:) + h*k1*1932/2197 - h*k2*7200/2197 + h*k3*7296/2197                               );
    k5 = f( x(k)+h      , y(k,:) + h*k1*439/216   - h*k2*8         + h*k3*3680/513  - h*k4*845/4104               );
    k6 = f( x(k)+h/2    , y(k,:) - h*k1*8/27      + h*k2*2         - h*k3*3544/2565 + h*k4*1859/4104 - h*k5*11/40 );
    
    e = abs(k1/360 - k3*128/4275 - k4*2197/75240 + k5/50 + k6*2/55);
  
//    if(auto_step > 0)
 //     if(e < minError)
//        h = h * 1.1;
//        continue;
//      elseif(e > maxError)
//        h = h * 0.5;
//        continue;
//      end;
//    end;

    hs = [hs; h]
  //y  = [y; y(k,:) + (  k1*25/216 + k3*1408/2565  + k4*2197/4104   - k5/5              )*h];
    y  = [y; y(k,:) + (  k1*16/135 + k3*6656/12825 + k4*28561/56430 - k5*9/50 + k6*2/55 )*h];
    x  = [x; x(k)   + h];
    k  = k + 1;
  end;
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
function [x, y, hs] = valida(metodo, testes, x0, xm, y0, y_exato, f)
  
  log_2 = log(2);
  m     = 10;

  old_auto_step = auto_step;
  auto_step     = 0;
  
  y_xm  = y_exato(xm);
  [x,y,hs] = metodo(x0, xm, m, y0, f);
  yk    = y(m + 1, 1);

  h     = ((xm - x0) / m);
  e_k   = abs(yk - y_xm);

  printf("\n");

  printf("     m     |       h      |     e_h(E)   |      2^q     |       q      \n");
  printf("-----------+--------------+--------------+--------------+--------------\n");
  printf(" %9d | %1.10f | %1.10f |      --      |      --      \n", m, h, e_k);

  for k = 1:max(testes, 2);
    h  = 0.5 * h;
    m  = 10.0 * 2^k

    [x, y] = metodo(x0, xm, m, y0, f);

    e_k   = abs(y(m + 1, 1) - y_xm);
    ratio = abs(yk - y_xm) / e_k;
    q_k   = abs(log(ratio) / log_2);

    printf(" %9d | %1.10f | %1.10f | %1.10f | %1.10f\n", m, h, e_k, ratio, q_k);

    yk = y(m + 1, :);
  end

  auto_step = old_auto_step;

endfunction
////////////////////////////////////////////////////////////////////////////////
function waitUser()
  printf("\n-> Digite `resume` para continuar ou `abort` para finalizar:");
  pause;
endfunction
////////////////////////////////////////////////////////////////////////////////
// Retrato de fase
////////////////////////////////////////////////////////////////////////////////
function plotRF(y)
  clf();
  plot2d(y(:,1), y(:,2),style=1);
endfunction
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// (1) Considerando-se o PVI abaixo:
//     /
//    | y''(t) = 4*y'(t) - 13*y(t)
//    | y (0) = -1
//    | y'(0) =  2
//    | t \in [0, 1]
//     \

// Solucao exata: y(t) = exp(2*t) * (4*sin(3*t)/3 - cos(3*t));

function v = lista05_ex1_exact(x)
  v = [];
  i = 1;
  for t = x
    v(i) = exp(2*t) * (4*sin(3*t)/3 - cos(3*t));
    i = i + 1;
  end
endfunction

// (1.a)
// Reescreve-se na forma normal (neste caso, vetorial)
//   z' = f(t,z), onde z = (z1, z2).
//  Fazendo z1 = y e z2 = y' vem:

// y''(t ) = 4 * y'(t) - 13 * y(t)
// y' (t0) = y'0;
// y  (t0) = y0;

// z1 = y
// z2 = y'

// z1' = y'  = z2
// z2' = y'' = 4 * y' - 13 * y

// f1(t, z1, z2) = z2(t);
// f2(t, z1, z2) = 4*z2(t) - 13*z1(t);

// z' = f(t, z) = (f1, f2) = (z1', z2')

// z(t0) = (z1(t0), z2(t0))
//       = (y0    , y'0   )

function v = lista05_ex1(t, y)
  k1 = y(2);
  k2 = 4*y(2) - 13*y(1);
  v  = [k1 k2];
endfunction

printf("\n\n(1.b) Validacao e aproximacoes:\n");
k_max = 8;
t0    = 0;
tm    = 1;
y0    = [-1 2];
[t,y] = valida(MetodoMidPoint, k_max, t0, tm, y0, lista05_ex1_exact, lista05_ex1);

waitUser();

printf("\n\n(1.c) Plot da solucoes `exata` e numerica;\n");
y_ext = lista05_ex1_exact(t');
y_num = y(:, 1);
clf();
plot(t, [y_ext y_num]);

waitUser();

printf("\n\n(1.d) Retrato de Fase em `tempo conveniente`;\n");
m  = 10*2^k_max;
t0 = 0;
y0 = [-1 2];
for tm = 1:5
  disp(tm , "tempo = ");
  [t, y] = MetodoMidPoint(t0, tm, m, y0, lista05_ex1);
  plotRF(y);
  waitUser();
end;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// (2) Considerando-se o PVI abaixo:
//     /
//    | y'(t) = 1 + (t - y) ^ 2
//    | y (2) = 1
//    | t \in [2, 3]
//     \

// Solucao exata: y(t) = t + 1 / (1 - t);

function v = lista05_ex2(t, y)
  v  = 1 + (t - y(1))^2;
endfunction;

function v = lista05_ex2_exact(x)
  v = [];
  i = 1;
  for t = x
    v(i) = t + 1 / (1 - t);
    i = i + 1;
  end
endfunction;

printf("\n\n(2.a) Erro Local;\n");
printf("      TODO =(\n");

printf("\n\n(2.b) Implementacao e graficos;\n");

k_max = 5;
t0    = 2;
tm    = 2.1;
y0    = 2;
m     = 10*2^k_max;
auto_step = 0;
[t, y, hs] = valida(rkf, k_max, t0, tm, y0, lista05_ex2_exact, lista05_ex2);

auto_step = 1;
[t, y, hs] = rkf(t0, tm, m, y0, lista05_ex2);

printf("\n\n     Solucoes `exata` e numerica;\n");
y_ext = lista05_ex2_exact(t');
y_num = y(:);
clf();
plot(t, [y_ext y_num]);

waitUser();

printf("\n\n     Serie de tempo dos Hs;\n");

clf();
plot(t, hs);

// [EoF]

