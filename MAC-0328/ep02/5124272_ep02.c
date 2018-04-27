/*
tmacedo
32016362
Thiago Pinheiro de Macedo
5124272
*/

/*
A tree is a well-known data structure that is either empty (null, void, nothing) 
or is a set of one or more nodes connected by directed edges between nodes 
satisfying the following properties.

    * There is exactly one node, called the root, to which no directed edges point.
    * Every node except the root has exactly one edge pointing to it.
    * There is a unique sequence of directed edges from the root to each node. 

For example, consider the illustrations below, in which nodes are represented 
by circles and edges are represented by lines with arrowheads. The first two 
of these are trees, but the last is not. 
6 8  5 3  5 2  6 4  5 6  0 0                  (is a tree)
8 1  7 3  6 2  8 9  7 5  7 4  7 8  7 6  0 0   (is a tree)
3 8  6 8  6 4  5 3  5 6  5 2  0 0             (is NOT a tree)
-1 -1
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
#define maxV      (50000)

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

/******************************************************************************
 * Definicoes de estruturas de base do modulo
 ******************************************************************************/
struct node {
  Vertex w;
  struct node* next;
};
typedef struct node* Node;

struct digraph {
  int   V;
  int   A;
  Node* adj;
};
typedef struct digraph* Digraph;

/******************************************************************************
 * Variaveis globais internas do modulo
 ******************************************************************************/
/* lista de cores utilizadas na pintura dos vertices */
static int time, parnt[maxV], d[maxV], f[maxV], arcIn[maxV];

/******************************************************************************
 * Node NewNode(Vertex w, Node next) 
 * Criacao de novo elemento da lista de vetices;
 ******************************************************************************/
Node NewNode(Vertex w, Node next) 
{
  Node x;

  x = (Node)malloc(sizeof *x);
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
  Vertex v;
  Digraph G;

  G = (Digraph)malloc(sizeof *G);
  G->V = V;
  G->A = 0;
  G->adj = (Node*)malloc(V * sizeof(Node));
  for(v = 0; v < G->V; v++)
  {
    arcIn[v]  = 0;
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
  Node    p, l;
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
int DIGRAPHinsertA(Digraph G, Vertex v, Vertex w)
{
  Node p;

  /* verifica consistencia dos parametros recebidos
     (extremos e arcos em si mesmo) */
  if(v == w || v >= G->V || w >= G->V) 
  {
    return FALSE;
  }
  /* verifica se o arco ja nao existe no digrafo */
  for(p = G->adj[v]; p != NULL; p = p->next)
  {
    if(p->w == w) 
    {
      return FALSE;
    }
  }
  /* arco valido: aloca e atribui */
  G->adj[v] = NewNode(w, G->adj[v]);
  G->A++;
  return TRUE;
}
/******************************************************************************
 * int cycleR (Digraph G, Vertex v) 
 * Verifica recursivamente a presenca de ciclo em um digrafo;
 ******************************************************************************/
int cycleR (Digraph G, Vertex v) 
{
  Node p;

  d[v] = time++;
  for (p = G->adj[v]; p != NULL; p = p->next)
  {
    if (d[p->w] == -1) 
    {
      parnt[p->w] = v;
      if (cycleR(G, p->w) == 1) 
      {
        return TRUE;
      }
    }
    else if (f[p->w] == -1) 
    {
      return TRUE;
    }
  }
  f[v] = time++;
  return FALSE;
}
/******************************************************************************
 * int DIGRAPHuniqueRoot(Digraph G)
 * Verifica existencia de ciclos num digrafo (preenche estrutura de parent);
 ******************************************************************************/
int DIGRAPHcycle(Digraph G) 
{
  Vertex v;

  /* inicializacao */
  time = 0;
  for (v = 0; v < G->V; v++) 
  {
    d[v] = f[v] = -1;
    parnt[v] = -1;
  }

  for (v = 0; v < G->V; v++)
  {
    if (d[v] == -1)
    {
      /* Marca parent do vertice como ele proprio (root) */
      parnt[v] = v;
      if (cycleR(G,v) == 1)
      {
        return TRUE;
      }
    }
  }
  return FALSE;
}
/******************************************************************************
 * int DIGRAPHuniqueRoot(Digraph G)
 * Verifica unicidade da raiz de um digrafo (utiliza estrutura de parent);
 ******************************************************************************/
int DIGRAPHuniqueRoot(Digraph G)
{
  Vertex v, x, root = -1;

  for(v = 0; v < G->V; v++)
  {
    if(d[v]+1 < f[v])
    {
      x = v; 
      while(parnt[x] != x)
      {
        x = parnt[x];
      }
      if(root < 0)
        root = x;
      else if(root != x)
        return FALSE;
    }
  }
  return TRUE;
}
/******************************************************************************
 * int DIGRAPHuniqueInbound(Digraph G)
 * Verifica unicidade dos caminhos entrantes num dado digrafo;
 ******************************************************************************/
int DIGRAPHuniqueInbound(Digraph G)
{
  Vertex v;

  for(v = 0; v < G->V; v++)
  {
    /* Nao podem existir vertices com mais de um caminho entrante; */
    if(arcIn[v] > 1)
    {
      return FALSE;
    }
  }
  return TRUE;
}
/******************************************************************************
 * int DIGRAPHisTree(Digraph G)
 * Verifica se o digrafo eh uma arvore/arborecencia;
 ******************************************************************************/
int DIGRAPHisTree(Digraph G)
{
  /* vazio eh uma arvore; */
  if(G->A > 0)
  {
    if(!DIGRAPHuniqueInbound(G)) return FALSE;
    if(DIGRAPHcycle(G))          return FALSE;
    if(!DIGRAPHuniqueRoot(G))    return FALSE;
  }
  return TRUE;
}
/******************************************************************************
 * int main(void)
 * Funcao principal e ponto de entrada do modulo;
 ******************************************************************************/
int main(void)
{
  Digraph G;
  Vertex  v, w;
  int     k;
  int     stop, reset, isTree;

  k = 0;
  stop = FALSE;
  while(!stop)
  {
    reset = FALSE;
    isTree = TRUE;
    G = DIGRAPHinit(maxV);
    while(!stop && !reset && scanf("%d %d", &v, &w) == 2)
    {
      if(v < 0 || w < 0)
      {
        stop = TRUE;
      }
      else if(v == 0 && w == 0)
      {
        k++;
        /* Efetura teste de bicolor; */
        if(isTree && DIGRAPHisTree(G))
        {
          printf("Case %d is a tree.\n", k);
        }
        else
        {
          printf("Case %d is not a tree.\n", k);
        }
        reset = TRUE;
      }
      else if(isTree)
      {
        isTree = DIGRAPHinsertA(G, v, w);
        if(isTree)
        {
          /* acumula numero de caminhos entrantes */
          arcIn[w]++;
        }
      }
    }
    DIGRAPHfree(G);
  }
  return 0;
}
