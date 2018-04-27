// Thiago Pinheiro de Macedo - NUSP 5124272

// Operacao:
// -> Acrescenta ao final de "y", a linha "k", somada a linha "x"
// y = [y; y(k,:) + x]

// Metodo de Heun (Runge-Kutta 1 estagio ordem 1;
// Funcao original (e otima ideia): 
// eulerA (euler aprimorado) por Eduard Oda
function [t, y] = MetodoHeun(t0, tm, m, y0, f)
  h   = (tm - t0) / m;
  h_2 = 0.5 * h;
  t   = t0;
  y   = y0;
  for k = 1:m
    k1 = f(t(k), y(k,:));
    k2 = f(t(k) + h, y(k,:) + h * k1);
    y  = [y; y(k,:) + h * (k1 + k2) / 2];
    t  = [t; t(k) + h];
  end
endfunction

