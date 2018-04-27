// Thiago Pinheiro de Macedo - NUSP 5124272

// Renova o ambiente =o)
clear
// Metodo de Euler Explicito:
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

// Funcao dy/dt original do problema de cauchy;
function ret = f(t, y)
  ret = -y;
endfunction;

// Solucao exata do problema de cauchy;
function ret = y_exata(t)
  ret = exp(t_0 - t);
endfunction;

// Metodo de Euler Explicito;
function y_k1 = Euler(t_k, y_k, h)
  y_k1 = y_k + h * f(t_k, y_k);
endfunction;

// Definicao da etapa de processamento (parte 1 ou 2 do exercicio);
etapa = input("Informe a etapa (1 ou 2):");
if(etapa == 1)
  // 1.) Para t \in [0, 100], empregando passos de integracao
  //     dt = (100 - 0) / 2^i ,  i=0...5;
  t_m = 100;
  disp(t_m);
elseif(etapa == 2)
  // 2.) Para t "tendendo a infinito", empregando os mesmos passos de
  //     integracao do item anterior;
  t_m = input("Informe o valor de t_m:");
end;

m = input("Informe as divisoes no intervalo (t_m - t_0):");

// Processamento
t_0 = 0;
y_0 = 1;
t_size = (t_m - t_0);
dt = t_size / m;
t  = [t_0];
y  = [y_0];
z  = [y_0];
for(k = 1:m)
  y(k+1) = Euler(t(k), y(k), dt);
  z(k+1) = y_exata(t(k));
  t(k+1) = t(k) + dt;
end;

// Plotagem dos dados
x = [y z];
clf();
plot(t, x);

// TODO :
// - Laco para repetir os inputs (como fazer em scilab?!)
