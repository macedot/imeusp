/*
tmacedo
32016362
Thiago Pinheiro de Macedo
5124272
*/
/******************************************************************************
 * Inclusoes da LIB padrao 
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>

/******************************************************************************
 * Definicoes de estruturas de base do modulo;
 ******************************************************************************/
enum {FALSE = 0, TRUE = 1};

typedef int     Vertex;
typedef Vertex  ItemFila;
typedef int     Cost;
  
 typedef struct link {
	Vertex  w;
	Cost    cst;
	struct link* next;
} *Link;

typedef struct digraph {
	int     V;
	int     A;
	Cost**  adj;
} *Digraph;

/******************************************************************************
 * Definicoes internas do modulo;
 ******************************************************************************/
#define ZERO_COST   0 
#define GRAPHinit   DIGRAPHinit
#define Graph       Digraph
#define MY_MIN(x,y) ( (x) < (y) ? (x) : (y) )
#define MY_MAX(x,y) ( (x) > (y) ? (x) : (y) )

/******************************************************************************
 * Variaveis globais internas do modulo;
 ******************************************************************************/
static Cost maxCST;

/******************************************************************************
 * Cost** MATRIXint(int r, int c, Cost val);
 * A função abaixo aloca uma matriz com linhas 0..r-1 e colunas 0..c-1. 
 * Cada elemento da matriz recebe valor val.
 ******************************************************************************/
Cost** MATRIXint(int r, int c, Cost val) 
{ 
   Vertex  i, j;
   Cost**  m;

   m = (Cost**)malloc(r * sizeof(Cost*));

   for(i = 0; i < r; i++)
      m[i] = (Cost*)malloc(c * sizeof(Cost));

   for(i = 0; i < r; i++)
      for(j = 0; j < c; j++)
         m[i][j] = val;

   return m;
}
/******************************************************************************
 * Digraph DIGRAPHinit(int nNumVertex);
 * Inicializacao de novo drigrafo;
 ******************************************************************************/
Digraph DIGRAPHinit(int nNumVertex) 
{ 
   Digraph G;

   G      = malloc(sizeof *G);
   G->V   = nNumVertex; 
   G->A   = 0;
   G->adj = MATRIXint(nNumVertex, nNumVertex, maxCST);
   return G;
}
/******************************************************************************
 * void MATRIXfree(Cost** matrix, int nSize);
 * Liberacao de matrix;
 ******************************************************************************/
void MATRIXfree(Cost** matrix, int nSize)
{
	Vertex  v;

	for(v = 0; v < nSize; v++)
	{
		free(matrix[v]);
	}
	free(matrix);
}
/******************************************************************************
 * void DIGRAPHfree(Digraph G);
 * Liberacao de drigrafo alocado em DIGRAPHinit e seus respectivos elementos;
 ******************************************************************************/
void DIGRAPHfree(Digraph G) 
{
	MATRIXfree(G->adj, G->V);
	free(G);
}
/******************************************************************************
 * void DIGRAPHinsertA(Digraph G, Vertex v, Vertex w, Cost cst);
 ******************************************************************************/
void DIGRAPHinsertA(Digraph G, Vertex v, Vertex w, Cost cst) 
{ 
   if(v != w && G->adj[v][w] == maxCST) 
   {
      G->adj[v][w] = cst; 
      G->A++;
   }
}
/******************************************************************************
 * void GRAPHinsertE(Graph G, Vertex v, Vertex w, Cost cst);
 ******************************************************************************/
void GRAPHinsertE(Graph G, Vertex v, Vertex w, Cost cst)
{ 
	DIGRAPHinsertA(G, v, w, cst);
	DIGRAPHinsertA(G, w, v, cst);
}
/******************************************************************************
 * Floyd-Warshall;
 ******************************************************************************/
/******************************************************************************
 * Cost DIGRAPHallMinSpt(Digraph G, Vertex s, Vertex t);
 * Utiliza Floyd-Warshall para calcular melhor trajeto (caminho de maior fluxo);
 ******************************************************************************/
void DIGRAPHallMinSpt(Digraph G) 
{
	Vertex  v, w, k;

	/* Caso onde origem == destino; */
	for(v = 0; v < G->V; v++)
		G->adj[v][v] = 0;

	/* Floyd-Warshall */
	for(k = 0; k < G->V; k++)
		for(v = 0; v < G->V; v++)
			for(w = 0; w < G->V; w++)
				G->adj[v][w] = MY_MAX(G->adj[v][w], 
				                      MY_MIN(G->adj[v][k], G->adj[k][w]));

}
/******************************************************************************
 * void DIGRAPHdump(Digraph G)
 ******************************************************************************/
void DIGRAPHdump(Digraph G)
{
	Vertex v, w;

	printf("   ");
	for(w = 0; w < G->V; w++)
	{
		printf("|%3d", w);
	}
	printf("\n");
	printf("---");
	for(w = 0; w < G->V; w++)
	{
		printf("+---");
	}
	printf("\n");

	for(v = 0; v < G->V; v++)
	{
		printf("%3d", v);
		for(w = 0; w < G->V; w++)
		{
			printf("|%3d", G->adj[v][w]);
		}
		printf("\n");
	}
}
/******************************************************************************
 * int main(void);
 * Funcao principal e ponto de entrada do modulo;
 ******************************************************************************/
int main(void)
{
	Vertex   orig, dest;
	Cost     cost;
	Digraph  G;
	int      nCities, nRoads;
	int      nCase  , nCountTrip, nNumTransp;

	nCase = 1;
	while(scanf("%d %d", &nCities, &nRoads) == 2 
		  && nCities > 0 && nRoads > 0)
	{
		nCountTrip = 0;
		maxCST     = 0;

		G = DIGRAPHinit(nCities);

		while(nRoads-- > 0
			  && (scanf("%d %d %d", &orig, &dest, &cost) == 3))
		{
			orig--;
			dest--;
			GRAPHinsertE(G, orig, dest, cost);
		}

		if(scanf("%d %d %d", &orig, &dest, &nNumTransp) == 3)
		{
			orig--;
			dest--;
			nCountTrip = 0;

			if(orig != dest)
			{
				DIGRAPHallMinSpt(G);
				
				/* menor capacidade do melhor caminho,
				   descontado o uso de uma posicao para o guia; */
				cost = (G->adj[orig][dest] - 1);
				if(cost > 0)
				{
					/* numero de viagens necessaria... */
					nCountTrip = nNumTransp / cost;

					/* .. corrigido se nao eh um multiplo; */
					if(nNumTransp % cost)
						nCountTrip++;
				}
			}
			printf("Scenario #%d\n", nCase);
			printf("Minimum Number of Trips = %d\n", nCountTrip);
			printf("\n");
		}

		DIGRAPHfree(G);

		nCase++;
	}
	return 0;
}
/* EoF */
