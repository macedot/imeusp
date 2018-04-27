  format(11) ;	
  printf("Digite o número de seu CIC, sem o traço antes do dígito verificador e sem os pontos a cada três dígitos.\n") ;
  cic = scanf("%d\n") ; // Azar do usuário se digitou errado :-) //

  printf("user1 \t user2 \t i \t aux \t \t verificador1 \t verificador2\n") ;
  aux = cic ;
  // A linha seguinte serve para inicializar vari'aveis para serem imprimidas 
  user1 = 0 ; user2 = 0 ; i = 0 ; verificador1 = 0 ; verificador2 = 0 ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  user2 = modulo(aux,10) ; // Toma o resto da divisão de aux por 10
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  aux = floor(aux/10) ; // Faz a divisão inteira de aux por 10
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  user1 = modulo(aux,10) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  aux = floor(aux/10) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  
  verificador1 = 9*modulo(aux,10) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  verificador2 = 8*modulo(aux,10) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  aux = floor(aux/10) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  for (i=2:9),
    verificador1 = verificador1 + (10-i)*modulo(aux,10) ;
    printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
    verificador2 = verificador2 + (9-i)*modulo(aux,10) ;
    printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
    aux = floor(aux/10) ;
    printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  end ;
  
  verificador1 = modulo(verificador1,11) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  verificador1 = modulo(verificador1,10) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  verificador2 = 9*verificador1+verificador2 ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  verificador2 = modulo(verificador2,11) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;
  verificador2 = modulo(verificador2,10) ;
  printf("%d \t %d \t %d \t %11.0f \t \t %d \t %d\n",user1,user2,i,aux,verificador1,verificador2) ;

  if ((verificador1==user1)&(verificador2==user2)) then,
    printf("O número %11.0f é um CIC válido\n",cic) ;
  end ;
