//Fa�a uma fun��o que calcule o n-�simo termo da sequ�ncia abaixo:
//F1 = 1 ;
//F2 = 1 ;
//Fn = Fn-1 + Fn-2 ;
//Por exemplo, os 10 primeiros termos da sequ�ncia s�o: 
//1 1 2 3 5 8 13 21 34 55
//Prot�tipo da fun��o:
//function res = fibo(n) 
//Envie a tarefa junto com um programa que chame a fun��o.

function res = PrintFibo(n) 
	a = 0;
	b = 1;
	res = 1;
	i = 1;

  printf("1");

 	while(i<n)
	  res = a + b;
	  a = b;
	  b = res;
	  printf(", %d",res);
		i = i + 1;
	end;
endfunction;

nElementos = 0;
nFibo = 0;

printf("\n>>> S�rie de Fibonacci <<<\n");
printf("--- Digite o total de elementos da s�rie ou zero para sair:");
nElementos = scanf("%d");

if(nElementos>0)
  printf("\n");
  nUltimoFibo = PrintFibo(nElementos);
  printf("\n");
end;

