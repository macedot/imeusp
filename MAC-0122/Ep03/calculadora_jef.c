#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX 100

/* Estrutura do elemento da lista ligada*/ 
typedef struct elem{
   int digito;
   struct elem *prox;
}Elem;


/*Funcao para criar um elemento da lista ligada, a partir de
  uma informação recebida. Ela retorna um ponteiro para o elemento criado*/
Elem *cria_elem(int info){
   Elem *p; /*Ponteiro para o elemento criado*/
   p = (Elem *) malloc(sizeof (Elem));
   if (p == NULL){
      printf ("Nao foi possivel criar o elemento. \n");
      return (NULL);
   }
   else{
      p -> digito = info;
      p -> prox = NULL;
      return (p);
   }
}


/*Funcao para criar uma lista ligada com as informacoes recebidas do 
  arquivo de entrada. Ela retorna um ponteiro para o inicio da lista criada*/
Elem *cria_lista(char *nomeArq){
   FILE *pe;     /*Ponteiro para o arquivo de entrada*/
   Elem *inicio, /*Ponterio para o inicio da lista*/
        *fim;    /*Ponteiro para o final da lista*/
   int nread,    /*Variavel que recebe o numero lido do arquivo*/
       num;      /*Variavel que recebe o numero lido do arquivo 
                   tranformado em inteiro*/
   pe = fopen (nomeArq, "r");
   if (pe == NULL){ /*Condicao para verificar se o arquivo foi aberto*/   
       printf ("\nO arquivo nao pode ser aberto.\n");
       return (NULL);
   }
   else{
       inicio = NULL;
       fim = inicio;
       if (!feof(pe)){ 
            nread = fgetc(pe);
            if (isdigit(nread)){ /*Condicao pára verificar se a informacao 
				                   lida do arquivo e ou nao um numero*/
                num = nread - '0'; 
                inicio = cria_elem(num);
                fim = inicio;
            }
			else{ 			
               printf ("\nERRO. \n");
	           fclose (pe);
	           return (NULL);
			}
       }
       while (!feof(pe)){ /*Varifica se o arquivo nao esta no fim*/
              nread = fgetc(pe);
              if (isdigit(nread)){
                  num = nread - '0';
                  fim -> prox = cria_elem(num);
                  fim = fim -> prox;
              }
       } 
   }
   fclose (pe);
   return (inicio);   
}


/*Funcao que inverte uma lista. Ela retorna um ponteiro para o inicio da 
  lista invertida*/
Elem *inverte_lista(Elem *inicio){
    Elem *atual, 
         *aux,   
         *fim;    
    if (inicio == NULL) return (NULL);
    if (inicio -> prox == NULL) return (inicio);
    aux = inicio -> prox;
    inicio -> prox = NULL;
    fim = inicio;
    while (aux -> prox != NULL){
        atual = aux -> prox;
        aux -> prox = fim;
        fim = aux;
        aux = atual;
    }
    if (aux -> prox == NULL){
        aux -> prox = fim;
        fim = aux;
    }
    return (fim);
}


/*Funcao usada para eliminar os zeros a esquerda da lista ligada. 
  Ela retorna um ponteiro para o "novo" inicio da lista, ja sem os zeros 
  a esquerda*/
Elem *elim_zeros(Elem *q){
    Elem *atual; /*Ponteiro auxiliar*/
    atual = q;
    if (atual -> digito == 0){
		while (q -> digito == 0 && q->prox != NULL){
            q = q -> prox;
			free (atual);
            atual = q;
        }
    }
    return (atual);
} 


/*Funcao que recebe um ponteiro para o iniciode uma lisrta*/
void libera(Elem *p){
	Elem *aux; /*Ponteiro auxiliar, para que nao seja "perdida" a lista
	             ao liberar a memoria de cada elemento*/
	while (p != NULL){
		aux = p;
		p = p->prox;
		free (aux);
	}
}


/*Funcao que soma duas listas, e retorna um ponteiro para uma terceira 
  lista(criada) com o resultado*/
Elem *soma(Elem *n1, Elem *n2){ 
    Elem *insoma, /*Ponteiro para o inicio da lista com o resultado*/ 
         *fsoma;  /*Ponteiro para o final da lista com o resultado*/
    int naux,     /*Variavel auxiliar que recebe a soma de dois digitos*/
        resto,    /*Variavel auxiliar que recebe o resto da soma, de dois
				    digitos, dividida por 10*/
        vaium;    /*Variavel usada para saber se vai um na proxima soma 
	                entre dois digitos, se vai um ela recebe 1 e recebe 0
	                caso contrario*/
    naux = (n1 -> digito) + (n2 -> digito);
    if (naux > 9){
        resto = naux%10;
        insoma = cria_elem (resto);
        fsoma = insoma;
        naux = naux/10;
        fsoma->prox = cria_elem (naux);
        fsoma = fsoma->prox;
        n1 = n1->prox;
        n2 = n2 -> prox;
        vaium = 1; 
    }   
    else{
        insoma = cria_elem (naux);
        fsoma = insoma;
        n1 = n1->prox;
        n2 = n2 -> prox;
        vaium = 0;
    }
    while (n1 != NULL && n2 != NULL){
          if (vaium == 1) naux = fsoma->digito + (n1 -> digito) + (n2 -> digito);
          else{ 
              naux = (n1 -> digito) + (n2 -> digito);	
              fsoma->prox = cria_elem (0);
              fsoma = fsoma->prox;
          }
          if (naux > 9){
             resto = naux%10;
             fsoma->digito = resto; 
             naux = naux/10;
             fsoma->prox = cria_elem (naux);
             fsoma = fsoma->prox;
             vaium = 1;
          }
          else{ 
              fsoma->digito = naux;
              vaium = 0;
          }
          n1 = n1->prox;
          n2 = n2 -> prox;
    }
    if (n1 == NULL){ /*Condicao para saber se a primeira lista terminou 
		               antes da segunda*/
           while (n2 != NULL){
              if (vaium == 1) naux = fsoma->digito + (n2 -> digito);
              else{ 
                   naux = (n2 -> digito);
                   fsoma->prox = cria_elem (0);
                   fsoma = fsoma->prox;
               }
               if (naux > 9){
                    resto = naux%10;
                    fsoma->digito = resto;
                    naux = naux/10;
                    fsoma->prox = cria_elem (naux);
                    fsoma = fsoma->prox;
                    vaium = 1;
                }
                else{ 
                    fsoma->digito = naux;
                    vaium = 0;
                }
                n2 = n2 -> prox;
           }
    }
    if (n2 == NULL){ /*Condicao para saber se a segunda lista terminou 
		               antes da primeira*/
        while (n1 != NULL){
            if (vaium == 1) naux = fsoma->digito + (n1 -> digito);
            else{ 
                naux = (n1 -> digito);
                fsoma->prox = cria_elem (0);
                fsoma = fsoma->prox;
            }
            if (naux > 9){
                resto = naux%10;
                fsoma->digito = resto;
                naux = naux/10;
                fsoma->prox = cria_elem (naux);
                fsoma = fsoma->prox;
                vaium = 1;
            }
            else{ 
                fsoma->digito = naux;
                vaium = 0;
            }
            n1 = n1->prox;	
        }
    }
    return (insoma);
}


/*Funcao que multiplica duas listas, e retorna um ponteiro para uma terceira 
  lista com o resultado*/
Elem *mult(Elem *n1, Elem *n2){
    Elem *inmult, /*Ponterio para o inicio da lista com o resultado da 
		            multiplicacao*/          
         *fmult,  /*Ponterio para o final da lista com o resultado da 
		            multiplicacao*/
		 *aux,    /*Ponteiro auxiliar*/
         *p1;     /*Ponteiro auxiliar usado para "guardar" o inicio da lista um*/
    int resto,    /*Variavel que recebe o resto da multiplicacao, entre dois 
		            digitos, dividida por 10*/
		sobe,     /*Variavel que recebe o valor que sera somado na "proxima" 
		            multiplicacao entre dois digitos, ou seja, o valor que "sobe"*/
        prod;     /*Recebe o valor do produto entre dois digitos*/
	p1 = n1; sobe = 0;
	inmult = cria_elem (0);
	fmult = aux = inmult;
	if (n2->digito == 0){
		fmult->prox = cria_elem (0);
	    fmult = fmult->prox;			
		n2 = n2->prox;
	}
	if (n1 -> digito == 0){ 
		n1 = n1->prox;
		fmult = fmult->prox;
	}
	while (n2 != NULL && n1 != NULL){ /*Condicao para verificar se as listas 
		                                esta no fim*/
		while (n2->digito == 0){ /*Condicao que verifica se o digito a ser 
			                       multiplicado e zero, se for "pula" para o 
			                       proximo digito*/
            if (fmult->prox == NULL) fmult->prox = cria_elem (0);
		    fmult = fmult->prox;			
			n2 = n2->prox;
		}
		while (n1 -> digito == 0){
			if (fmult->prox == NULL) fmult->prox = cria_elem (0);
			fmult = fmult->prox;
			n1 = n1->prox;
		}
		while (n1 != NULL && n2 != NULL){
			while (n1 -> digito == 0){
				fmult->prox = cria_elem (0);
     			fmult = fmult->prox;
				n1 = n1->prox;
			}
			if (sobe != 0) prod = fmult->digito + n1->digito * n2->digito;
			else prod = fmult->digito + sobe + n1->digito * n2->digito;
	        if (prod > 9){
				resto = prod%10;
				if (fmult == NULL){
					aux->prox = cria_elem (resto);
					fmult = aux->prox;
				}
				else fmult->digito = resto;
				sobe = prod/10;
				if (fmult->prox == NULL){ 
					fmult->prox = cria_elem (sobe);
					fmult = fmult->prox;
				}
				else{
					fmult = fmult->prox;
					fmult->digito = fmult->digito + sobe;
				}
			}	
			else{
				fmult->digito = prod;
			    if (fmult->prox == NULL) fmult->prox = cria_elem (0);
		        fmult = fmult->prox;
			}
			if (n1->digito != 0) n1 = n1->prox;
		}
	    n2 = n2->prox; n1 = p1;
		aux = aux->prox;
		fmult = aux; sobe = 0;
	}
	return (inmult);
}


/*Funcao para escrever o um arquivo de saida, atraves de uma lista*/
int escreveArquivo (Elem *p, char *nomeArq){
	FILE *ps; /*Ponteiro para o arquivo de saida a ser criado*/
	ps = fopen (nomeArq, "w");
	if (ps == NULL){ /*Condicao para verificar se o arquivo foi aberto corretamente.*/
		printf ("\nERRO. \n");
		return 0;
	}
    if (p == NULL) return 0;
	else{
	     while (p != NULL){
		       fprintf (ps, "%d", p -> digito);
               p = p -> prox;
	      }
	}
	fclose (ps);
	return 1;
}


int main(int argc, char *argv[]){
	Elem *pn1,    /*Ponteiro para a lista do arquivo 1*/
	     *pn2,    /*Ponteiro para a lista do arquivo 2*/
	     *pnr;    /*Ponteiro para a lista resultante*/ 
	char N1[MAX], /*Recebe o nome do arquivo 1*/
	     N2[MAX], /*Recebe o nome do arquivo 2*/
	     NR[MAX]; /*Recebe o nome do arquivo resultante*/
	int i,        /*Usado como indice para fazer comparacoes*/
	    met,      /*Recebe o método a ser usado, soma ou mutiplicacao*/
        ok;       /*Usada para verificar se o arquivo de saida foi gravado corretamente*/
	if (argc != 9){  /*Condicao para verificar se os parametros passados pelo
			           usuario estao corretos.*/
		printf ("\nERRO.\n");
		printf ("Use o programa como descrito abaixo:\n");
		printf ("calculadora -m <operacao> -a <arquivo1> -b <arquivo2> "
			    "-o <arquivoResultado> \n");
		return 21;
	}
	else{
	    met = 0;
	    for (i = 0; i < argc; i++){ /*Aqui os nomes dos arquivos passados pelo usuário
									  são devidamente armazenados nas strings ou 
									  variaveis correspondentes.*/
	        if (argv[i][0] == '-'){
		        switch (argv[i][1]){
	                case 'm':
						met = atoi (argv[i+1]); 
                   	    break;
	                case 'a':
	                    strcpy (N1, argv[i+1]);
		                break;
                    case 'b':			 
                        strcpy (N2, argv[i+1]); 
                        break;
	                case 'o':
	                    strcpy (NR, argv[i+1]);
                        break;
                    default:
                        printf ("opcao invalida.\n");
                        printf ("Use o programa como descrito abaixo:\n");
		                printf ("calculadora -m <operacao> -a <arquivo1> "
			                    "-a <arquivo2> -o <arquivoResultado> \n");
                        return 21;
                }
            }
        }
        pn1 = cria_lista (N1);
		if (pn1 == NULL){ /*Condicao para verificar se a lista 1 foi crida*/
            printf ("O arquivo 1 contem caracteres ou e nulo. \n");
			return 1;
	    }
	    pn2 = cria_lista (N2);
        if (pn2 == NULL){ /*Condicao para verificar se a lista 2 foi crida*/
	         printf ("O arquivo 2 contem caracteres ou e nulo. \n");
	         return 1;
	    }
	    else{
	        pn1 = inverte_lista (pn1);
	        pn2 = inverte_lista (pn2);
	        if (pn1 == NULL || pn2 == NULL) return 1; /*Condicao para verificar se as 
			                                      listas foram invertidas corretamente*/
	        else{
	           switch (met){
			      case 1:			 
                     pnr = soma (pn1, pn2);
		             break;
                  case 2:
		             pnr = mult (pn1, pn2);
		             break;
	             default:
		             printf ("Opcao de metodo invalida. \n");
                     printf ("Use: \n");
                     printf ("1 para o Soma \n");   
                     printf ("2 para o Multiplicação \n");
		             return 21;
			   }
	        }
	        pnr = inverte_lista (pnr);
	        pnr = elim_zeros (pnr);
	        ok = escreveArquivo (pnr, NR);
			if (ok != 1){ /*Condicao para verificar se o arquvi foi gravado corretamente*/
               printf ("\nNao foi possivel gravar o arquivo com o resultado. \n");
	           return 1;
	        }
			libera (pnr); /*Aqui a memoria usada com as listas e liberada*/
		}
	    printf ("\nO arquivo contendo o resultado foi gravado com sucesso. \n\n");
    }
    system ("PAUSE");
    return 0;
}














