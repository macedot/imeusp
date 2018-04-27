/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Eduardo Oda 01/02/08
//
//
// DESCRICAO:
// =========
// Biblioteca de metodos para integracao numerica de sistemas de EDO's.
//
// Dado um problema de Cauchy
//
//    | dy/dx = f(x,y)
//    | y(x0) = y0
//    |
//    | x \in [x0,xm]
//    | y: R -> R^n
//    | f: RxR^n -> R^n
//
// todos os metodos tem a mesma assinatura:
//
//    metodo(x0,xm,m,y0,f)
//
// onde x0 e xm sao numeros reais, m eh um inteiro indicando o numero de
// passos, y0 eh um vetor linha com a condicao inicial, f eh o campo de
// vetores escrito como um vetor linha.
//
// Todos os metodos tem o mesmo tipo de retorno [x,y], onde x eh um vetor
// coluna ((m+1)x1) com os valores de x onde a solucao foi aproximada e y eh
// uma matriz (m+1)xn com as aproximacoes.
//
// Veja exemplos do uso destas rotinas em exemplos.sce.
// 
//
// MODO DE USO:
// ===========
// Salve este arquivo no mesmo diretorio de seu script e nele acrescente a
// o comando
//    getf('lib.sci')
//
// Se este arquivo e o seu script nao estiverem no mesmo diretorio, altere
// o comando acima indicando o caminho correto deste arquivo.
//
// Depois disso, basta chamar as funcoes definidas abaixo.
//
// NOTAS:
// =====
// Possivelmente roda em outras versoes de Scilab e em outros sistemas
// operacionais onde o Scilab rode. Mas estas rotinas foram testadas num
// Scilab 4.1.1 rodando numa estacao rodando Slackware Linux 12:
//    $ uname -a
//    Linux honir 2.6.19.1 #7 Fri Oct 19 00:22:45 BRST 2007 i686 Mobile AMD
//    Sempron(tm) Processor 2800+ AuthenticAMD GNU/Linux
//
// PRE-REQUISITOS:
// ==============
//      - Scilab 4.1.1
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// METODO DE RUNGE-KUTTA 4 ESTAGIOS E ORDEM 4
/////////////////////////////////////////////////////////////////////////////
function [x,y]=rk44(x0,xm,m,y0,f)
  h=(xm-x0)/m;
  x=x0;
  y=y0;
  for k=1:m
    k1=f( x(k)     ,  y(k,:)        );
    k2=f( x(k)+h/2 ,  y(k,:)+h*k1/2 );
    k3=f( x(k)+h/2 ,  y(k,:)+h*k2/2 );
    k4=f( x(k)+h   ,  y(k,:)+h*k3   );
    y=[y; y(k,:) + (k1 + 2*k2 + 2*k3 + k4)*h/6];
    x=[x; x(k)+h];
  end
endfunction

/////////////////////////////////////////////////////////////////////////////
// METODO DE EULER APRIMORADO
/////////////////////////////////////////////////////////////////////////////
function [x,y]=eulerA(x0,xm,m,y0,f)
  h=(xm-x0)/m;
  x=x0;
  y=y0;
  for k=1:m
    k1=f(x(k),y(k,:));
    k2=f(x(k)+h,y(k,:)+h*k1);
    y=[y;y(k,:)+h*(k1+k2)/2];
    x=[x;x(k)+h];
  end
endfunction

/////////////////////////////////////////////////////////////////////////////
// METODO DE EULER
/////////////////////////////////////////////////////////////////////////////
function [x,y]=euler(x0,xm,m,y0,f)
  h=(xm-x0)/m;
  x=x0;
  y=y0;
  for k=1:m
    y=[y;y(k,:)+h*f(x(k),y(k,:))];
    x=[x;x(k)+h];
  end
endfunction

/////////////////////////////////////////////////////////////////////////////
// METODO DE RUNGE-KUTTA-FEHLBERG
//
// Para validar este metodo comente o ajuste de passo e escolha a y referente
// ao Runge-Kutta que deseja validar.
//
// O ajuste de passo eh feito de maneira bem ingenua. Controles melhores
// podem ser encontrados na Internet
//
/////////////////////////////////////////////////////////////////////////////
function [x,y]=rkf(x0,xm,m,y0,f)
  h=(xm-x0)/m;
  x=x0;
  y=y0;
  k=1;
  hs=[];

  while x(k)<xm
    k1=f( x(k)          ,  y(k,:)                                                                                      );
    k2=f( x(k)+h/4      ,  y(k,:) + h*k1/4                                                                             );
    k3=f( x(k)+h*3/8    ,  y(k,:) + h*k1*3/32       + h*k2*9/32                                                        );
    k4=f( x(k)+h*12/13  ,  y(k,:) + h*k1*1932/2197  - h*k2*7200/2197  + h*k3*7296/2197                                 );
    k5=f( x(k)+h        ,  y(k,:) + h*k1*439/216    - h*k2*8          + h*k3*3680/513   - h*k4*845/4104                );
    k6=f( x(k)+h/2      ,  y(k,:) - h*k1*8/27       + h*k2*2          - h*k3*3544/2565  + h*k4*1859/4104  - h*k5*11/40 );
    
    e=abs(k1/360-k3*128/4275-k4*2197/75240+k5/50+k6*2/55);
  
    if(e<10^(-10))
      h=h*1.1;
      continue;
    elseif(e>10^(-8))
      h=h/2;
      continue;
    end
    
    hs=[hs;h]
    y=[y; y(k,:) + (  k1*25/216 + k3*1408/2565  + k4*2197/4104   - k5/5              )*h];
//    y=[y; y(k,:) + (  k1*16/135 + k3*6656/12825 + k4*28561/56430 - k5*9/50 + k6*2/55 )*h];
    x=[x; x(k)+h];
    k=k+1;
  end
  disp(hs);
endfunction


//////////////////////////////////////
// VALIDA OS METODOS
//////////////////////////////////////
function valida(metodo,testes,x0,xm,y0,y_exato,f)
  y_xm=y_exato(xm);
  [x,y]=metodo(x0,xm,10,y0,f);
  yi=y(11,1);
  for i=1:testes
    [x,y]=metodo(x0,xm,10*2^i,y0,f);
    disp(abs(yi-y_xm)/abs(y(10*2^i+1,1)-y_xm));
    yi=y(10*2^i+1,1);
  end
endfunction
