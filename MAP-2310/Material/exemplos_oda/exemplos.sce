/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Eduardo Oda 01/02/08
//
//
// DESCRICAO:
// =========
// Exemplos de uso dos metodos e modelos implementados em lib.sci e
// modelos.sci.
//
//
// MODO DE USO:
// ===========
// Salve este arquivo no mesmo diretorio dos arquivos lib.sci e modelos.sci.
// 
// Abra o Scilab e utilize o comando 'cd' para chegar ate o diretorio onde
// vc salvou os arquivos.
//
// Clique no botao 'editor' da interface do Scilab. No abra este arquivo e
// descomente o exemplo que deseja rodar. Salve depos clique em
// Execute -> Load into Scilab
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
// Prepara o ambiente
/////////////////////////////////////////////////////////////////////////////
clear;
getf("lib.sci");
getf("modelos.sci");



/////////////////////////////////////////////////////////////////////////////
// Faz a validacao dos metodos utilizando o problema do exercicio 12
/////////////////////////////////////////////////////////////////////////////
//valida(rkf,7,0,1,[-1,2],y_EX12,f_EX12)
//valida(rk44,10,0,1,[-1,2],y_EX12,f_EX12)
//valida(eulerA,9,0,1,[-1,2],y_EX12,f_EX12)
//valida(euler,11,0,1,[-1,2],y_EX12,f_EX12)



/////////////////////////////////////////////////////////////////////////////
// Faz o retrato de fase do modelo de Lotka-Volterra
/////////////////////////////////////////////////////////////////////////////
clf();
// condicoes iniciais
CI=[
  2,2;
  3,2;
  4,2;
  5,2;
  6,2;
];
[m,n]=size(CI);
for i=1:m
  // se desejar utilizar outro metodo, basta editar a linha seguinte:
  [x,y]=rk44(0,8,1000,CI(i,:),f_LV);
  plot2d(y(:,1),y(:,2),rect=[0,0,7,9],style=5);
end



/////////////////////////////////////////////////////////////////////////////
// Faz o retrato de fase do problema do exercicio 12
/////////////////////////////////////////////////////////////////////////////
//clf();
//CI=[
//  1,0;
//  0,1;
//  -1,0;
//  0,-1;
//  cos(2*%pi/3),sin(2*%pi/3);
//  cos(-%pi/3),sin(-%pi/3);
//];
//[m,n]=size(CI);
//for i=1:m
//  [x,y]=rk44(0,2,100,CI(i,:),f_EX12);
//  plot2d(y(:,1),y(:,2),rect=[-20,-20,20,20],style=5);
//end



/////////////////////////////////////////////////////////////////////////////
// Utiliza o metodo de Runge-Kutta-Fehlberg para o problema do exercicio 18
/////////////////////////////////////////////////////////////////////////////
//clf();
//[x,y]=rkf(2,3,100,1,f_EX18);
//plot2d(x,y);

