/*****************************************************************************
 * Metodo de Euler Explicito
 * 
 * Eduardo Oda - jan/2008
 * 
 * Esta eh uma implementacao do Metodo de Euler:
 * 	| y_0
 * 	| y_{k+1}=y_k+h*f(x_k,y_k)
 * 	
 * para integrar um problema de Cauchy:
 *  	| dy/dx = f(x,y)
 *  	| y(x_0) = y_0
 *  	| x \in [x_0,x_m]
 *  	| y \in R
 * 
 * O programa recebe como parametros o intervalo [x_0,x_m], a condicao inicial
 * y_0 e a quantidade de passos m. O tamanho do passo eh calculado como
 * (x_m-x_0)/m. A funcao f deve ser fornecida diretamente no codigo. Para isso,
 * edite a funcao
 * 
 * 	double f(double x, double y){...}
 * 
 * A saida do programa sao dois arquivos euler.out e euler.plot, gravados no
 * mesmo diretorio onde o comando foi chamado, sobreescrevendo arquivos
 * existentes.
 * 
 * O arquivo euler.out eh uma tabela com duas colunas, a primeira eh o ponto x_k
 * e a segunda eh valor aproximado de y(x_k), ou seja, y_k.
 * 
 * O arquivo euler.plot eh um script do gnuplot que plota o resultado numerico
 * da integracaoe pode ser vista com o comando:
 * 	$ gnuplot euler.plot
 *
 * Este arquivo ja é gravado como um cabecalho de shell script, assim, se vc
 * der permissao de execucao para ele, ele roda esse comando automaticamente.
 * 
 * Para compilar este programa utilize o comando:
 * 	$ gcc -Wall -ansi -lm euler.c -o euler
 * 
 * E para rodar o programa:
 * 	$ euler [-plot] m x_0 x_m y_0
 * Oparametro opcional -plot chama o gnuplot automaticamente.
 *
 * Este programa foi escrito no padrao ANSI, portanto deve rodar em qualquer
 * maquina com configuracoes usuais, mas foi testado em:
 * 
 * - Slackware Linux 12
 * 	$ uname -a
 * 	Linux honir 2.6.19.1 #7 Fri Oct 19 00:22:45 BRST 2007 i686 Mobile AMD
 * 	Sempron(tm) Processor 2800+ AuthenticAMD GNU/Linux
 *
 * Pre-requisitos:
 * 	- gcc 4.1.2 ou superior
 * 	- gnuplot 4.2 ou superior
 * 
 *****************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * Declaração da Função Derivada
 *****************************************************************************/
double f(double x, double y){
	return(y-pow(x,2.0)+1.0);
}

int main(int argc,char *argv[]){
	int m, i, j;
	double x_0,x_m, y_0, h, x_k, y_k;
	FILE *out, *plot;

	/* 
	 * Converte os parametros
	 */
	if(argc<5){
		printf("Modo de uso:\n\t$ euler [-plot] m x_0 x_m y_0\n\n");
		return(1);
	}
	i=(argc==6)?1:0;
	m=atoi(argv[1+i]);
	x_0=atof(argv[2+i]);
	x_m=atof(argv[3+i]);
	y_0=atof(argv[4+i]);

	/* 
	 * Calcula do Passo de Integração 
	 */
	h=(x_m-x_0)/m;

	/* 
	 * Inicializa os variaveis e executa o metodo, mandado os resultados
	 * diretamente para a saida
	 */
	out=fopen("euler.out","w");
	fprintf(out,"# x_k\t\t  y_k\n");
	x_k=x_0;
	y_k=y_0;
	fprintf(out,"%lf\t%lf\n",x_k, y_k);
	for(j=0;j<m;j++){
		y_k+=h*f(x_k,y_k);
		x_k+=h;
		fprintf(out,"%lf\t%lf\n",x_k, y_k);
	}
	fclose(out);

	/*
	 * Cria o script do gnuplot
	 */
	plot=fopen("euler.plot","w");
	fprintf(plot,"#!/usr/bin/gnuplot -persist\n\n");
	fprintf(plot,"plot [%lf:%lf] \"euler.out\" title \"y_k\" with linespoints",x_0, x_m);
	fclose(plot);

	/* 
	 * Roda o gnuplot caso receba o parametro -plot
	 */
	if(i)
		system("gnuplot -persist euler.plot");

	return(0);
}

