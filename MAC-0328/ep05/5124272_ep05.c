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
 * Definicoes internas do modulo;
 ******************************************************************************/
#define NUM_DIGIT  4
#define RADIX      10
#define MAX_VERTEX 10000 /* pow(RADIX, NUM_DIGIT); */

/******************************************************************************
 * Definicoes de estruturas de base do modulo;
 ******************************************************************************/
enum {FALSE = 0, TRUE = 1};

typedef int      Vertex;
typedef Vertex   ItemFila;
  
 typedef struct link {
	Vertex w;
	struct link* next;
} *Link;

typedef struct digraph {
	int		V;
	int		A;
	Link*	adj;
} *Digraph;

/******************************************************************************
 * Variaveis globais internas do modulo;
 ******************************************************************************/
/* variaveis do BFS; */
static int dist[MAX_VERTEX];
static Vertex parnt[MAX_VERTEX];
/* variaveis da fila do BFS; */
static ItemFila  *q;
static int inicio, fim;

/******************************************************************************
 * char* Vertex2Str(Vertex value, char* szOutput)
 * Converte um vertex em string terminada em zero (BASE DECIMAL);
 ******************************************************************************/
char* Vertex2Str(Vertex value, char* szOutput)
{
	/* TODO : Generalizar com parametro NUM_DIGIT; */
	sprintf(szOutput,"%04d", value);
	return szOutput;
}
/******************************************************************************
 * Vertex Str2Vertex(int value, char* szOutput)
 * Converte uma string terminada em zero (BASE DECIMAL) em vertex;
 ******************************************************************************/
Vertex Str2Vertex(char* szValue)
{
	return (Vertex)atoi(szValue);
}
/******************************************************************************
 * Link NewLink(Vertex w, Link next);
 * Criacao de novo elemento da lista de vetices;
 ******************************************************************************/
Link NewLink(Vertex w, Link next) 
{
	Link  x;

	x = (Link)malloc(sizeof *x);
	x->w    = w;
	x->next = next;
	return x;
}
/******************************************************************************
 * Digraph DIGRAPHinit_ep05(void);
 * Inicializacao de novo drigrafo especifico deste caso;
 ******************************************************************************/
Digraph DIGRAPHinit_ep05(void) 
{
	char     szVertex[NUM_DIGIT+1];
	char     szMate  [NUM_DIGIT+1];
	Digraph  G;
	Vertex   v;
	int      i;

	G = (Digraph)malloc(sizeof *G);
	G->V = MAX_VERTEX;
	G->A = 0;
	G->adj = (Link*)malloc(MAX_VERTEX * sizeof(Link*));

	/* referencias */
	for(v = 0; v < G->V; v++)
	{
		G->adj[v] = NULL;
		Vertex2Str(v, szVertex);
		memcpy(szMate, szVertex, sizeof(szVertex));
		for(i = 0; i < NUM_DIGIT; i++)
		{
			/* TODO : Aqui RADIX eh ignorado por simplicidade;
			          Criar definicoes de valor maximo e minimo; */
			szMate[i] = szVertex[i] + 1;
			if(szMate[i] > '9') szMate[i] = '0';
			G->adj[v] = NewLink(Str2Vertex(szMate), G->adj[v]);
			G->A++;
			
			szMate[i] = szVertex[i] - 1;
			if(szMate[i] < '0') szMate[i] = '9';
			G->adj[v] = NewLink(Str2Vertex(szMate), G->adj[v]);
			G->A++;
			
			/* restaura estado inicial; */
			szMate[i] = szVertex[i];
		}
	}

	return G;
}
/******************************************************************************
 * void DIGRAPHfree(Digraph G);
 * Liberacao de drigrafo alocado em DIGRAPHinit e seus respectivos elementos;
 ******************************************************************************/
void DIGRAPHfree(Digraph G) 
{
	Vertex  v;
	Link    p, l;

	for(v = 0; v < G->V; v++)
	{
		p = G->adj[v];
		while(p != NULL)
		{
			l = p;
			p = p->next;
			free(l);
		}
	}
	free(G);
}
/******************************************************************************
 * int DIGRAPHremoveV(Digraph G, Vertex v)
 * remove arestastas com origem num determinado vertice 
 * (QUASE o mesmo que remover o vertice. Note que as arestas com DESTINO
 *  a este vertice sao MANTIDAS!);
 ******************************************************************************/
int DIGRAPHremoveV(Digraph G, Vertex v)
{
	Link  p, l;

	p = G->adj[v];
	while(p != NULL)
	{
		l = p;
		p = p->next;
		free(l);
	}
	G->adj[v] = NULL;
	return FALSE;
}
/******************************************************************************
 * Gerenciamento de Fila visto em aula;
 ******************************************************************************/
/******************************************************************************
 * void QUEUEinit(int maxN) ;
 * Inicializa a fila no tamanho especificado;
 ******************************************************************************/
void QUEUEinit(int maxN) 
{
	q = (ItemFila*)malloc(maxN * sizeof(ItemFila));
	inicio = fim = 0;
}
/******************************************************************************
 * int QUEUEempty(void) ;
 * Verifica se a fila esta vazia. Retorna TRUE se vazia, FALSE caso contrario;
 ******************************************************************************/
int QUEUEempty(void) 
{
	return (inicio >= fim ? TRUE : FALSE);
}
/******************************************************************************
 * void QUEUEput(ItemFila item);
 * Insere novo elemento na fila;
 ******************************************************************************/
void QUEUEput(ItemFila item)
{
	q[fim++] = item;
}
/******************************************************************************
 * ItemFila QUEUEget(void) ;
 * Obtem primeiro elemento da fila;
 ******************************************************************************/
ItemFila QUEUEget(void) 
{
	return q[inicio++];
}
/******************************************************************************
 * void QUEUEfree(void) ;
 * Libera fila alocada;
 ******************************************************************************/
void QUEUEfree(void) 
{
	free(q);  q = NULL;
}
/******************************************************************************
 * void DIGRAPHbfs (Digraph G, Vertex s)
 * Busca em largura com calculo da distancia de s aos outro vertices;
 ******************************************************************************/
void DIGRAPHbfs(Digraph G, Vertex s)
{
	Vertex v; 
	Link   p;

	for (v = 0; v < G->V; v++)
	{
		dist[v] = -1;
	}
	QUEUEinit(G->V);
	dist[s] = 0;
	parnt[s] = s;
	QUEUEput(s);
	while(!QUEUEempty()) 
	{
		v = QUEUEget();
		for(p = G->adj[v]; p != NULL; p = p->next)
		{
			if(dist[p->w] == -1) 
			{
				dist[p->w] = dist[v] + 1;
				parnt[p->w] = v;
				QUEUEput(p->w);
			}
		}
	}
	QUEUEfree();
}
/******************************************************************************
 * void DIGRAPHdump(Digraph G)
 ******************************************************************************/
#if (0)
void DIGRAPHdump(Digraph G)
{
	Vertex v;
	Link   p;

	for(v = 0; v < G->V; v++)
	{
		printf("%04d -> ", v);
		for(p = G->adj[v]; p != NULL; p = p->next)
		{
			printf(" %04d", p->w);
		}
		printf("\n");
	}
}
#endif
/******************************************************************************
 * Vertex ReadVertex(void);
 * Le um Vertex da stdin no formato "X X X X" e retorna o respectivo Vertex;
 ******************************************************************************/
Vertex ReadVertex(void)
{
	char szNumber[NUM_DIGIT+1];
	int  i, aux;

	szNumber[NUM_DIGIT] = 0;
	for(i = 0; i < NUM_DIGIT; i++)
	{
		scanf("%d ", &aux);
		szNumber[i] = '0' + aux;
	}
	return Str2Vertex(szNumber);
}
/******************************************************************************
 * int main(void);
 * Funcao principal e ponto de entrada do modulo;
 ******************************************************************************/
int main(void)
{
	Vertex   orig, dest, forb;
	Digraph  G;
	int      nCountTest, nCountForb;

	/* processa casos de teste */
	scanf("%d", &nCountTest);
	while(nCountTest-- > 0)
	{
		/* Obtem origem e destino desejado */
		orig = ReadVertex();
		dest = ReadVertex();
		/* Incializa estrutura padrao do digrafo neste caso; */
		G = DIGRAPHinit_ep05();
		/* processa vertex proibidos */
		scanf("%d", &nCountForb);
		while(nCountForb-- > 0)
		{
			forb = ReadVertex();
			DIGRAPHremoveV(G, forb);
		
		}
		/* realiza busca em largura na origem desejada; */
		DIGRAPHbfs(G, orig);
		/* imprime distancia ao ponto desejado; */
		printf("%d\n", dist[dest]);
		/* libera alocacao atual da estrutura do digrafo; */
		DIGRAPHfree(G);
	}
	return 0;
}
/* EoF */
