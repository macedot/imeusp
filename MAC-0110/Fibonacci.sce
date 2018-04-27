//Fa�a uma fun��o que calcule o n-�simo termo da sequ�ncia abaixo:
//F1 = 1 ;
//F2 = 1 ;
//Fn = Fn-1 + Fn-2 ;
//Por exemplo, os 10 primeiros termos da sequ�ncia s�o:
//1 1 2 3 5 8 13 21 34 55
//Prot�tipo da fun��o:
//function res = fibo(n) 
//Envie a tarefa junto com um programa que chame a fun��o.

function res = fibo(n) 
	a = 1;
	b = 1;
	res = 1;
	i = 2;
	while(i<n)
	  res = a + b;
	  a = b;
	  b = res;		
		i = i + 1;
	end;
endfunction;

nPos = 1;
nFibo = 1;

printf("\n>>> S�rie de Fibonacci <<<\n");

while(nPos>0)
  printf("--- Digite a posi��o do elemento na s�rie (1 a n) ou zero para sair:");
  nPos = scanf("%d");
  if(nPos>0) then,
    nFibo = fibo(nPos);
    printf("\n");
    printf("Posi��o %d => Elemento %d\n",nPos,nFibo);
  end;
end;

