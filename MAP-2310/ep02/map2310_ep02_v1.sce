clear
clf();

// Esta eh uma implementacao do Metodo de Euler:
//
// /
// | y_0 dado
// | y_{k+1} = y_k + h * f(t_k, y_k)
// \
// 	
// para integrar um problema de Cauchy:
// /
// | dy/dt  = f(t, y(t))
// | y(t_0) = y_0
// \
// 
// Particularmente, temos que f(t, y(t)) = -y(t), t_0 = 0 e y_0 = 1, 
// resultando no seguinte problema de Cauchy:
//
// /
// | dy/dt  = -y(t)
// | y(0)   = 1
// \
//

function ret = f(t, y)
  ret = -y;
endfunction;

function ret = f_exata(t)
  ret = exp(t_0 - t);
endfunction;

function [t,y] = Euler(FuncaoF, t_0, t_m, h, y_0)
  y = [y_0];
  for(i = 1:m)
    y(i+1) = y(i) + h * FuncaoF(t(i), y(i));
    t(i+1) = t(i) + h;
  end;
endfunction;

// 1.) Para t \in [0, 100], empregando passos de integração 
//     dt = (100 - 0) / 2^i ,  i=0...5;
t_0 = 5;
t_m = 15;
y_0 = 1;
t = [];
y = [];
for(i = 5:5)
  dt = (t_m - t_0) / 2^i;
  m  = (t_m - t_0) / dt;
  t  = [t_0];
  y  = [y_0];
  z  = [y_0];
  for(i = 1:m)
    y(i+1) = y(i) + dt * f(t(i), y(i));
    z(i+1) = f_exata(t(i));
    t(i+1) = t(i) + dt;
  end;
  clf();
  x = [y;z];
  plot(t, x);
  //t(i) = t1;
  //y(i) = y1;
//   [t(i+1), y(i+1)] = [c1, c2];
//  plot(t,y);
end;


// 2.) Para t "tendendo a infinito", empregando os mesmos passos de integração do item anterior;



