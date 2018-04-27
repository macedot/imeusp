// Lotka-Volterra
// Para rodar, abra o scilab e 
// exec("exemplo.sci");

// Limpa o ambiente
clear
clf();

// Define a funcao f
function v=f(x,y)
  v=[2*y(1)-y(1)*y(2);y(1)*y(2)-y(2)];
endfunction

// [x0,xm] intervalo de integracao
// m eh o numero de passos
// y0 eh vetor linha de condicoes iniciais
function [x,y]=euler(x0,xm,m,y0)
  h=(xm-x0)/m;
  x=x0;
  y=y0;
  e=0
  for k=1:m
    y=[y,y(:,k)+h*f(x(k),y(:,k))];
    x=[x,x(k)+h];
  end
endfunction

///////////////////////////
// Alguns exemplos de plot
///////////////////////////

// Plota retrato de fase
function plotRF(x,y)
  plot(y(1,:),y(2,:));
endfunction

// Plota grafico
function plotGr(x,y,l)
  plot2d(x',y',leg=l);
endfunction

// Plota grafico do erro'
function plotGrErro(x,y,y_exato)
  plot2d(x',abs(y-y_exato)');
endfunction

// Chama o metodo
[x,y]=euler(0,10,100,[2;2]);
plotRF(y);
pause + resume
plotRF(y);

