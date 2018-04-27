//Faça uma função que calcule o n-ésimo termo da sequência abaixo:
//F1 = 1 ;
//F2 = 1 ;
//Fn = Fn-1 + Fn-2 ;
//Por exemplo, os 10 primeiros termos da sequência são:
//1 1 2 3 5 8 13 21 34 55
//Protótipo da função:
//function res = fibo(n) 
//Envie a tarefa junto com um programa que chame a função.

function FiboVet(v,n) 
	a = 1;  
	b = 1;
  v(1) = 1;
  v(2) = 1;
	i = 2;
	while(i<n)
	  v(i) = a + b;
	  a = b;
	  b = v(i);		
		i = i + 1;
	end;
endfunction;

nPos = 0;
printf("\n>>> Série de Fibonacci - versão Vetor <<<\n");
printf("--- Digite a posição do elemento na série (1 a n) ou zero para sair:");
nPos = scanf("%d");

vetor = [nPos];

if(nPos>0) then
  FiboVet(vetor,nPos);
  printf("%d",vetor(1));
  for(i=2 : nPos)
    printf(",%d",vetor(i));
  end;
end;


