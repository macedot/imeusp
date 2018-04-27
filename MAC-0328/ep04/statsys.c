#include <stdio.h>
#include <sys/types.h>  /* para os tipos usados pela estrutura |stat| */
#include <sys/stat.h>   /* estrutura retornada por |stat| */


/* 
 * Apresentamos aqui um exemplo simples do uso da chamada do
 * sistema stat.  O programa recebe, através da linha de comando, 
 * uma lista de nomes de arquivos e, para cada um destes arquivos, 
 * imprime o instante em que em que o arquivo foi modificado
 * pela última vez. A saída será enviada para a saída-padrão (stdout).
 * 
 * Teste o programa com 
 * 
 *   meu_prompt>./statsys *
 *
 * por exemplo.
 */

int 
main (int argc, char *argv[])
{
  int k;
  struct stat bufst;
  char *nome_prog = argv[0];

  if (argc == 1) 
    {
      fprintf(stdout,"Uso: %s file1 file2 ... filek\n", nome_prog); 
      return 0;
    }
  
  for (k=1; --argc; k++)
    {
      /* 
       * Instante da última modificação de um arquivo.  Antes de olharmos
       * para o instante em que o arquivo de nome apontado por argv[k] foi
       * modificado pela última vez verificaremos se o arquivo esta acessível.
       * Caso o arquivo de nome apontado por argv[k] não exista então uma
       * mensagem de erro será enviada para a saída padrão de erro (stderr).
       * Se o arquivo puder ser acessado então o seu tempo de modificação será
       * escrito na saída padrão.
       */

      if (stat(argv[k], &bufst) == -1) 
	fprintf(stderr,"%s: arquivo %s não pode ser acessado.\n", nome_prog,
		argv[k]);
      else 
	fprintf(stdout,"%12ld %s\n", bufst.st_mtime, argv[k]); 

    }

  return 0;
}
