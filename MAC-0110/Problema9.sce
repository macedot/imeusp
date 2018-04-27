//Adriane Yonamine   n� 5124140
//Leandro Luiz       n� 5124122
//Soraia Mendes      n� 5124060
//Thiago Macedo      n� 5124272
//
//Problema 9: Dados tr�s n�meros positivos, a, b e c, lados de um tri�ngulo, 
//fa�a uma fun��o que retorne 1 se os n�meros s�o os lados de um tri�ngulo ret�ngulo, ou 0, se n�o forem.
//Obs: N�o use a fun��o de raiz quadrada pois pode haver problema de arredondamento.
//Prot�tipo: function res = retangulo(a,b,c) 
function res = retangulo(a,b,c)
  if (a*a==b*b+c*c) then,
    res = 1;
  elseif (b*b==c*c+a*a) then,
    res = 1;
  elseif (c*c==a*a+b*b) then,
    res = 1;
  else,
    res = 0;   
  end;
endfunction;

L1=0;
L2=0;
L3=0;

printf ("\nDigite 3 lados de um triangulo\n");

L1=scanf ("%d"); 
L2=scanf ("%d");
L3=scanf ("%d");

res = retangulo(L1,L2,L3); 

if (res==1) then,
  printf ("o triangulo e retangulo\n");
else,
  printf ("o triangulo nao e retangulo\n");
end;  



