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
#include <memory.h>
#include <malloc.h>

/******************************************************************************
 * Definicoes internas do modulo 
 ******************************************************************************/
#define maxV      (500)
#ifndef FALSE
#define FALSE     (0)
#endif
#ifndef TRUE
#define TRUE      (1)
#endif

/******************************************************************************
 * Definicoes de relacao entre funcoes para digrafos e grafos;
 ******************************************************************************/
#define Vertex    int
#define graph     digraph
#define Graph     Digraph

#define GRAPHinit DIGRAPHinit
#define GRAPHshow DIGRAPHshow
#define GRAPHfree DIGRAPHfree

/******************************************************************************
 * Definicoes de estruturas de base do modulo
 ******************************************************************************/
struct node {
  Vertex w;
  struct node* next;
};
typedef struct node* link;

struct digraph {
  int   V;
  int   A;
  link* adj;
};
typedef struct digraph* Digraph;

/******************************************************************************
 * Variaveis globais internas do modulo
 ******************************************************************************/
/* lista de cores utilizadas na pintura dos vertices */
static int paint[maxV];

/******************************************************************************
 * link NEW(Vertex w, link next) 
 * Criacao de novo elemento da lista de vetices;
 ******************************************************************************/
link NEW(Vertex w, link next) 
{
  link x;

  x = (link)malloc(sizeof *x);
  x->w = w;
  x->next = next;
  return x;
}
/******************************************************************************
 * Digraph DIGRAPHinit(Vertex V) 
 * Inicializacao de novo drigrafo com V vertices;
 ******************************************************************************/
Digraph DIGRAPHinit(Vertex V) 
{
  Vertex  v;
  Digraph G;

  G = (Digraph)malloc(sizeof *G);
  G->V = V;
  G->A = 0;
  G->adj = (link*)malloc(V * sizeof(link));
  for(v = 0; v < V; v++)
  {
    G->adj[v] = NULL;
  }
  return G;
}
/******************************************************************************
 * void DIGRAPHfree(Digraph G) 
 * Liberacao de drigrafo alocado em DIGRAPHinit e seus respectivos elementos;
 ******************************************************************************/
void DIGRAPHfree(Digraph G) 
{
  link    p, l;
  Vertex  v;

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
 * void DIGRAPHinsertA(Digraph G, Vertex v, Vertex w)
 * Inclusao de novo elemento ARESTA v-w num digrafo G existente e valido;
 ******************************************************************************/
void DIGRAPHinsertA(Digraph G, Vertex v, Vertex w)
{
  link p;

  /* verifica consistencia dos parametros recebidos
     (extremos e arcos em si mesmo) */
  if(v == w || v < 0 || w < 0 || v >= G->V || w >= G->V) 
  {
    return;
  }
  /* verifica se o arco ja nao existe no digrafo */
  for(p = G->adj[v]; p != NULL; p = p->next)
  {
    if(p->w == w) 
    {
      return;
    }
  }
  /* arco valido: aloca e atribui */
  G->adj[v] = NEW(w, G->adj[v]);
  G->A++;
}
/******************************************************************************
 * void GRAPHinsertE(Graph G, Vertex v, Vertex w)
 * Inclusao de novo elemento ARESTA v-w num grafo G existente e valido;
 ******************************************************************************/
void GRAPHinsertE(Graph G, Vertex v, Vertex w)
{
  DIGRAPHinsertA(G, v, w);
  DIGRAPHinsertA(G, w, v);
}
/******************************************************************************
 * int colorR(Digraph G, Vertex v, int color)
 * Recebido um digrafo G valido, e um vertice v contido em G, pinta o vertice
 * na cor color e tenta pinta vertices adjacentes na cor !color ("inversa").
 * Caso um vertice ja esteja pintado numa cor diferente, retorna FALSE;
 * Retorna TRUE quando todos os vertices foram pintados sem conflitos;
 ******************************************************************************/
int colorR(Digraph G, Vertex v, int color)
{
  link p;

  paint[v] = color;
  for(p = G->adj[v]; p != NULL; p=p->next)
  {
    if(paint[p->w] == -1) 
    {
      if(colorR(G, p->w, !color) == 0)
      {
        return FALSE;
      }
    }
    else if(paint[p->w] == color)
    {
      return FALSE;
    }
  }
  return TRUE;
}
/******************************************************************************
 * int DIGRAPHbicolor(Digraph G)
 * Colore um digrafo G valido, iniciando pelo elemento 0, com a cor 0;
 ******************************************************************************/
int DIGRAPHbicolor(Digraph G)
{
  Vertex v;

  for(v = 0; v < G->V; v++) 
  {
    paint[v] = -1;
  }
  return colorR(G, 0, 0);
}
/******************************************************************************
 * int main(void)
 * Funcao principal e ponto de entrada do modulo;
 ******************************************************************************/
int main(void)
{
  Vertex v, w;
  Graph  G;
  int    V;
  int    A;

  /* Leitura do numero de vertices; */
  while(scanf("%d", &V) == 1 && V > 0)
  {
    G = GRAPHinit(V);
    /* Leitura do numero de arestas; */
    if(scanf("%d", &A) == 1 && A > 0)
    {
      /* Leitura das arestas/arcos; */
      while(A > 0 && scanf("%d %d", &v, &w) == 2)
      {
        /* Utilizando a boa fe do enunciado, erros de insersao podem
           ser despresados, pois os assume-se que os dados sao compativeis; */
        GRAPHinsertE(G, v, w);
        A--; 
      }
    }
    /* Efetura teste de bicolor; */
    if(DIGRAPHbicolor(G))
    {
      printf("BICOLORABLE.\n");
    }
    else
    {
      printf("NOT BICOLORABLE.\n");
    }
    GRAPHfree(G);
  }
  return 0;
}
