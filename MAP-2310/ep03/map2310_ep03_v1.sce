////////////////////////////////////////////////////////////////////////////////
// Thiago Pinheiro de Macedo - NUSP 5124272
////////////////////////////////////////////////////////////////////////////////

// Renova o ambiente =o)
clear
clf();

// Metodo de Heun (Runge-Kutta 2 estagios ordem 2);
function [t, y] = MetodoHeun(t0, tm, m, y0, f)
  h   = (tm - t0) / m;
  h_2 = 0.5 * h;
  t   = t0;
  y   = y0;
  for k = 1:m
    tk1 = t(k) + h;
    k1  = f(t(k) , y(k,:));
    k2  = f(tk1  , y(k,:) + h * k1);
    yk1 = y(k,:) + h_2 * (k1 + k2);
    y   = [y ; yk1];
    t   = [t ; tk1];
  end
endfunction

// Retrato de fase
function plotRF(y)
  plot2d(y(:,1), y(:,2),style=0);
endfunction

////////////////////////////////////////////////////////////////////////////////
// EXERCÍCIO #13 (2008-04-03. Upload no PACA em 2008-04-17):
//    Considerando-se a equação do Oscilador de Van der Pol:
//     /
//    | y''(t) = -k * y(t) + w * (1 - y(t)^2) * y'(t)
//    | y (t0) = y0
//    | y'(t0) = y0
//     \
//    pede-se:

// 1. Reescreva-a na forma normal (neste caso, vetorial)
//    z'=f(t,z), onde z=(z1,z2).
//    Sugestão: faça z1=y e z2=y'.

// -> Seguiu em aula que:
// y''(t) = -k * y(t) + w*(1-y^2(t))*y'(t);
// y'(t0) = y'0;
// y (t0) = y0;

// z1 = y
// z2 = y'

// z1' = y'  = z2
// z2' = y'' = -k * z1 + w*(1-z1^2(t)) * z2

// f1(t, z1, z2) = z2;
// f2(t, z1, z2) = -k * z1(t) + w * (1 - z1(t)^2) * z2(t);

// z' = f(t, z) = (f1, f2) = (z1', z2')

// z(t0) = (z1(t0), z2(t0))
//       = (y0    , y'0   )

function v = van_der_pol(t, y)
  k1 = y(2);
  k2 = -k * y(1) + w*(1 - y(1)^2) * y(2);
  v  = [k1 k2];
endfunction









////////////////////////////////////////////////////////////////////////////////
// 2. Para parâmetros físicos k=1 e w=3 e condições iniciais (i) e (ii),
//    resolva-a numericamente pelo Método de Heun. Sua resposta deve ser dada
//    pelo retrato de fase para estas duas condições iniciais

//   (isto é, trace num mesmo sistema de eixos a curva (z1(t),z2(t)),
//    t em [0,T] (você decide o T), para os dois conjuntos de condições iniciais).

// -> Dados k e w;
k  = 1;
w  = 3;
t0 = 0;
tm = 10;
m  = 2^12;

// (i)  y(0) = +2 , y'(0) = 0
[t, y] = MetodoHeun(t0, tm, m, [ 2 0], van_der_pol);
//plotRF(y);

// (ii) y(0) = -2 , y'(0) = 0
[t, y] = MetodoHeun(t0, tm, m, [-2 0], van_der_pol);
plotRF(y);
