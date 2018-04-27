#include <stdio.h>
#include <stdlib.h> /* para a função system */
#include <string.h> /* para strcat */ 

#define  MAX_CHARS 1024


/*
 * Apresentamos aqui um exemplo simples do uso da função de
 * biblioteca system.  O programa recebe, através da linha de comando, uma
 * lista de comandos, separados por um ponto-e-vírgula, e  usando a
 * função system executa um após outro em um shell.
 *
 * Para testar o programa execute-o, por exemplo, com o argumento 
 * "statsys *", se você já tiver compilado o programa statsys, 
 * ou seja, execute
 * 
 * meu_prompt> ./system statsys *
 */

int 
main (int argc, char *argv[])
{
  int k;
  char comando[MAX_CHARS];  
  char *nome_prog = argv[0]; /* argv[0] eh o nome do programa */

  if (argc == 1) 
    {
      fprintf(stdout,"Uso: %s comando1; comando2; ... comandok\n", nome_prog); 
      return 0;
    }

  for (k=1; k < argc; k++) 
    {
      /*  
       * Este trecho de programa compõe o próximo comando que 
       * será executado. Se  um comando não for o último então 
       * é assumido que ele termina com um ponto-e-virgula. 
       * Caso o comando tenha mais do que MAX_CHARS
       * caracteres uma mensagem de erro será enviada para a 
       * saída padrão de
       * erro (stderr).
       */

      comando[0] = '\0'; 
      
      for ( ; k < argc; k++) {
	if (strcmp(argv[k],";")==0) break;
	
	if (strlen(argv[k]) + strlen(comando) >= MAX_CHARS) 
	  { /* panic mode */
	    fprintf (stderr,"%s: comando %s... muito longo.\n", 
		     nome_prog,  comando);
	    
	    while (k < argc) 
	      if (strcmp(argv[k],";") == 0) break;
	      else k++;
	  }
	else {
	  strcat(comando," ");
	  strcat(comando,argv[k]);
	}
      }          

      /* 
       * No próximo tercho de executaremos o comando contido no vetor
       * 'comando'.  Tudo se passa como se a string 'comando' tivesse 
       * sido digitada como um comando em um shell.  
       */
    
      if (system(comando) != 0) 
	fprintf(stderr,"%s: erro encontrado ao executar o comando %s\n", 
                nome_prog, comando);
      else 
	fprintf(stdout,"\n%s: comando `%s' terminou normalmente.\n", argv[0],
		comando);
    }

  return 0;
}

