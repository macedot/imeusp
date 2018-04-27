//------------------------------------------------------------------------------
//Nome...: Thiago Pinheiro de Macedo
//Nº USP.: 5124272
//Turma..: 47
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//E1: Verifica quantos pontos estao na circunferencia, de centro [0,0] e raio 1;
function pi = meuPi(n) 
  pi=0;				  //resultado aproximado de pi

  if(n>0) then 
    x=0;  y=0; //coordenadas dos pontos gerados
    Nc=0;      //contador de pontos que pertencem ao circulo

    for(i=1 : n)
      //gera coordenadas aleatorias
      x=rand();
      y=rand();
      if(((x*x)+(y*y))<=1) then, //o ponto esta no circulo ?
        Nc=Nc+1; //conta mais um ponto dentro
      end;
    end;
    pi = 4 * (Nc/n); //PI ~ 4 * (Nc/n)
  end;
endfunction;
//-----------------------------------------------------------------------------
//E2: Calculo simples da media dos valores de um vetor de tamanho n;
function media = minhaMedia(vpi,n)
  media=0; //media dos valores do vetor

  if(n>0) then 
    //somamos os valores do vetor...
    for(i=1 : n)
      media = media + vpi(i);
    end;
    //.. e dividimos pelo numero de elementos
    media = media / n;
  end;
endfunction;
//------------------------------------------------------------------------------
//E3: Calcular a variancia nao-viciada dos n numeros do vetor vpi
function variancia = minhaVar(vpi,n)
  variancia = 0;

  if(n>1) then,
    media = minhaMedia(vpi,n); //calcula a media de valores do vetor
    tmp = 0;
    soma = 0;
    
    for(i=1 : n)
      tmp = vpi(i) - media;
      soma = soma + (tmp * tmp);
    end;
    variancia = (soma/(n-1));
  end;
endfunction;
//------------------------------------------------------------------------------
//E4: Programa pricnipal para estiamtiva de pi pelo metodo de monte carlo
num1 = 0;
num2 = 0;

printf("\n>>> Estimativa de PI (método de Monte Carlo) <<<");
//le um numero valido de pontos para o grafico
while((num1<1)|(num1>50)),
  printf("\nNúmero de pontos para o gráfico (1 a 50): "); 
  num1 = 50; // scanf("%d");
  if((num1<1)|(num1>50)) then,
    printf("\nO número digitado é inválido!!"); 
  end;
end;
//le um numero valido de interacoes para o calculo de media pi
while((num2<1)|(num2>100)),
  printf("\nInterações para obter a média de vetores (1 a 100): "); 
  num2 = 100; // scanf("%d");
  if((num2<1)|(num2>100)) then,
    printf("\nO número digitado é inválido!!"); 
  end;
end;
//prepara vetores de resultados
mediaPi = [num1];
varPi = [num1];
valorPi = [num2];
//efetua o calculo das medias e variancias
printf("\n");
for(i=1 : num1)
  //para mostrarmos que o programa esta rodando...
  printf("\tmediaPi(%d) = meuPi(%d) * %d;\n",i,i*1000,num2);
  for(j=1 : num2)
    valorPi(j) = meuPi(i*1000);
  end;
  mediaPi(i) = minhaMedia(valorPi,num2);
  varPi(i) = sqrt(minhaVar(valorPi,num2)); //Addendum: sqrt(), 29/10/2k4
end;
//------------------------------------------------------------------------------
//E5: Grafico de mediaPi, via plot2d() (multiplos de 1000) (barra de erros em errbar())
x=[1:1000:num1*1000];
plot2d(x,mediaPi,style=[-1],rect=[0,min(mediaPi)-0.01,num1*1000,max(mediaPi)+0.01] ); 
errbar(x,mediaPi,varPi,varPi);
//------------------------------------------------------------------------------
//[EoF]
