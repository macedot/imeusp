/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Eduardo Oda 01/02/08
//
//
// DESCRICAO:
// =========
// Arquivo de modelos.
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
// Cada uma das funcoes abaixo implementa f para um modelo. Todas as funcoes
// tem a mesma assinatura:
//
//    f(x,y)
//
// onde x eh um numero real e y eh um vetor linha de dimensao n.
//
// Todos as funcoes retorman um vetor linha de dimensao n.
//
// Quando disponivel a solucao exata de um modelo, havera uma funcao y que
// recebe como parametro um numero real x e retorna um vetor linha.
//
// Veja exemplos do uso destas rotinas em exemplos.sce.
// 
//
// MODO DE USO:
// ===========
// Salve este arquivo no mesmo diretorio de seu script e nele acrescente a
// o comando
//    getf('modelos.sci')
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
// MODELO SIS
/////////////////////////////////////////////////////////////////////////////
function v=f_SIS(x,y)
  // [x,y]=rk4(0,5,100,[500,5]);
  i=0.001;
  c=0.1;
  v=[-i*y(1)*y(2)+c*y(2),i*y(1)*y(2)-c*y(2)];
endfunction

/////////////////////////////////////////////////////////////////////////////
// MODELO SIR
/////////////////////////////////////////////////////////////////////////////
function v=f_SIR(x,y)
  // [x,y]=rk4(0,100,1000,[500,5,0]);
  i=0.001;
  c=0.1;
  v=[-i*y(1)*y(2),i*y(1)*y(2)-c*y(2),c*y(2)];
endfunction

/////////////////////////////////////////////////////////////////////////////
// MODELO LOTKA-VOLTERRA
/////////////////////////////////////////////////////////////////////////////
function v=f_LV(x,y)
  //[x,y]=rk4(0,10,100,[2,2]);
  v=[2*y(1)-y(1)*y(2),y(1)*y(2)-y(2)];
endfunction

/////////////////////////////////////////////////////////////////////////////
// MODELO FOCO REPULSOR
/////////////////////////////////////////////////////////////////////////////
function v=f_FR(x,y)
  v=[y(1)+y(2),y(1)-y(2)];
endfunction
// solucao exata para y(0)=[1;0]
function v=y_FR(x)
  v=exp(x)*[cos(x),-sin(x)];
endfunction

/////////////////////////////////////////////////////////////////////////////
// MODELO EXERCICIO 12
/////////////////////////////////////////////////////////////////////////////
function v=f_EX12(x,y)
  v=[y(2),-13*y(1)+4*y(2)];
endfunction
// solucao exata para [y(0),y'(0)]=[-1,2]
function v=y_EX12(x)
  v=exp(2*x).*(-cos(3*x)+4*sin(3*x)/3);
endfunction

/////////////////////////////////////////////////////////////////////////////
// MODELO EXERCICIO 18
/////////////////////////////////////////////////////////////////////////////
function v=f_EX18(x,y)
  v=1+(x-y)^2;
endfunction
// solucao exata para y(0)=1
function v=y_EX18(x)
  v=x'+1/(1-x);
endfunction

