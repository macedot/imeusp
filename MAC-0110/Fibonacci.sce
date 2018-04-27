//Faça uma função que calcule o n-ésimo termo da sequência abaixo:
//F1 = 1 ;
//F2 = 1 ;
//Fn = Fn-1 + Fn-2 ;
//Por exemplo, os 10 primeiros termos da sequência são:
//1 1 2 3 5 8 13 21 34 55
//Protótipo da função:
//function res = fibo(n) 
//Envie a tarefa junto com um programa que chame a função.

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

printf("\n>>> Série de Fibonacci <<<\n");

while(nPos>0)
  printf("--- Digite a posição do elemento na série (1 a n) ou zero para sair:");
  nPos = scanf("%d");
  if(nPos>0) then,
    nFibo = fibo(nPos);
    printf("\n");
    printf("Posição %d => Elemento %d\n",nPos,nFibo);
  end;
end;

