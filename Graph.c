//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023, Nov/Dec 2024
//
// Graph - Using a list of adjacency lists representation
//

// Student Name : Francisco Matos 
// Student Number : 113726
// Student Name : Luís Rodrigues
// Student Number : 114560

/*** COMPLETE THE GraphCreateTranspose FUNCTION ***/

#include "Graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "SortedList.h"

struct _Vertex {
  unsigned int id;
  unsigned int inDegree;
  unsigned int outDegree;
  List* edgesList;
};

struct _Edge {
  unsigned int adjVertex;
  double weight;
};

struct _GraphHeader {
  int isDigraph;
  int isComplete;
  int isWeighted;
  unsigned int numVertices;
  unsigned int numEdges;
  List* verticesList;
};

typedef struct _Edge GraphAdjacencyNode;



GraphAdjacencyNode* GraphAdjacencyListGetFirst(List* edgesList) {
  assert(edgesList != NULL);
  ListMoveToHead(edgesList);
  return ListGetCurrentItem(edgesList);
}


GraphAdjacencyNode* GraphAdjacencyListGetNext(List* edgesList) {
  assert(edgesList != NULL);
  ListMoveToNext(edgesList);
  return ListGetCurrentItem(edgesList);
}


unsigned int GraphAdjacencyNodeGetVertex(GraphAdjacencyNode* node) {
  assert(node != NULL);
  return node->adjVertex;
}



// The comparator for the VERTICES LIST

int graphVerticesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Vertex*)p1)->id;
  unsigned int v2 = ((struct _Vertex*)p2)->id;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

// The comparator for the EDGES LISTS

int graphEdgesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Edge*)p1)->adjVertex;
  unsigned int v2 = ((struct _Edge*)p2)->adjVertex;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

Graph* GraphCreate(unsigned int numVertices, int isDigraph, int isWeighted) {
  Graph* g = (Graph*)malloc(sizeof(struct _GraphHeader));
  if (g == NULL) abort();

  g->isDigraph = isDigraph;
  g->isComplete = 0;
  g->isWeighted = isWeighted;

  g->numVertices = numVertices;
  g->numEdges = 0;

  g->verticesList = ListCreate(graphVerticesComparator);

  for (unsigned int i = 0; i < numVertices; i++) {
    struct _Vertex* v = (struct _Vertex*)malloc(sizeof(struct _Vertex));
    if (v == NULL) abort();

    v->id = i;
    v->inDegree = 0;
    v->outDegree = 0;

    v->edgesList = ListCreate(graphEdgesComparator);

    ListInsert(g->verticesList, v);
  }

  assert((int)g->numVertices == ListGetSize(g->verticesList));

  return g;
}

Graph* GraphCreateComplete(unsigned int numVertices, int isDigraph) {
  Graph* g = GraphCreate(numVertices, isDigraph, 0);

  g->isComplete = 1;

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    List* edges = v->edgesList;
    for (unsigned int j = 0; j < g->numVertices; j++) {
      if (i == j) {
        continue;
      }
      struct _Edge* new = (struct _Edge*)malloc(sizeof(struct _Edge));
      if (new == NULL) abort();
      new->adjVertex = j;
      new->weight = 1;

      ListInsert(edges, new);
    }
    if (g->isDigraph) {
      v->inDegree = g->numVertices - 1;
      v->outDegree = g->numVertices - 1;
    } else {
      v->outDegree = g->numVertices - 1;
    }
  }
  if (g->isDigraph) {
    g->numEdges = numVertices * (numVertices - 1);
  } else {
    g->numEdges = numVertices * (numVertices - 1) / 2;
  }

  return g;
}

// Create the transpose of a directed graph
// This function should never be called on an undirected graph
// This function should never be called on a complete graph
Graph* GraphCreateTranspose(const Graph* g) {
  assert(g != NULL);
  assert(g->isDigraph);
  assert(g->isComplete == 0);

  // COMPLETE THE CODE
  // Criar um novo grafo com o mesmo número de vértices
  Graph* transposedGraph = GraphCreate(g->numVertices, g->isDigraph, g->isWeighted);
  // Iterar sobre os vértices do grafo original
  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  for (unsigned int i = 0; i < g->numVertices; i++) {
    struct _Vertex* currentVertex = ListGetCurrentItem(vertices);
    // Iterar sobre as arestas do vértice atual
    List* edges = currentVertex->edgesList;
    ListMoveToHead(edges);
    for (unsigned int j = 0; j < ListGetSize(edges); j++) {
      struct _Edge* edge = ListGetCurrentItem(edges);
      // Adicionar a aresta transposta ao novo grafo
      if (g->isWeighted) {
        GraphAddWeightedEdge(transposedGraph, edge->adjVertex, currentVertex->id, edge->weight);
      } else {
        GraphAddEdge(transposedGraph, edge->adjVertex, currentVertex->id);
      }
      ListMoveToNext(edges);
    }
    ListMoveToNext(vertices);
  }
  return transposedGraph;
}

void GraphDestroy(Graph** p) {
  assert(*p != NULL);
  Graph* g = *p;

  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices) == 0) {
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
      struct _Vertex* v = ListGetCurrentItem(vertices);
      List* edges = v->edgesList;
      if (ListIsEmpty(edges) == 0) {
        int i = 0;
        ListMoveToHead(edges);
        for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
          struct _Edge* e = ListGetCurrentItem(edges);
          free(e);
        }
      }
      ListDestroy(&(v->edgesList));
      free(v);
    }
  }
  ListDestroy(&(g->verticesList));
  free(g);

  *p = NULL;
}

// Read a graph from file
// Using the simple graph format of Sedgewick and Wayne
// Input argument must be a valid FILE POINTER
// File must be openend and closed by the caller
Graph* GraphFromFile(FILE* f) {
  assert(f != NULL);

  unsigned int isDigraph;
  fscanf(f, "%u", &isDigraph);

  unsigned int isWeighted;
  fscanf(f, "%u", &isWeighted);

  unsigned int numVertices;
  fscanf(f, "%u", &numVertices);

  unsigned int numEdges;
  fscanf(f, "%u", &numEdges);

  Graph* g = GraphCreate(numVertices, isDigraph, isWeighted);

  // Read the edges and add them to the graph
  unsigned int start_vertex;
  unsigned int end_vertex;
  double weight;

  if (isWeighted == 0) {
    for (unsigned int i = 0; i < numEdges; i++) {
      fscanf(f, "%u", &start_vertex);
      fscanf(f, "%u", &end_vertex);
      GraphAddEdge(g, start_vertex, end_vertex);
    }
  } else {
    for (unsigned int i = 0; i < numEdges; i++) {
      fscanf(f, "%u", &start_vertex);
      fscanf(f, "%u", &end_vertex);
      fscanf(f, "%lf", &weight);
      GraphAddWeightedEdge(g, start_vertex, end_vertex, weight);
    }
  }

  assert(numEdges == g->numEdges);

  return g;
}

// Graph

int GraphIsDigraph(const Graph* g) { return g->isDigraph; }

int GraphIsComplete(const Graph* g) { return g->isComplete; }

int GraphIsWeighted(const Graph* g) { return g->isWeighted; }

unsigned int GraphGetNumVertices(const Graph* g) { return g->numVertices; }

unsigned int GraphGetNumEdges(const Graph* g) { return g->numEdges; }

//
// For a graph
//
double GraphGetAverageDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return 2.0 * (double)g->numEdges / (double)g->numVertices;
}

static unsigned int _GetMaxDegree(const Graph* g) {
  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices)) return 0;

  unsigned int maxDegree = 0;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    if (v->outDegree > maxDegree) {
      maxDegree = v->outDegree;
    }
  }
  return maxDegree;
}

//
// For a graph
//
unsigned int GraphGetMaxDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return _GetMaxDegree(g);
}

//
// For a digraph
//
unsigned int GraphGetMaxOutDegree(const Graph* g) {
  assert(g->isDigraph == 1);
  return _GetMaxDegree(g);
}

// Vertices

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by indices of the adjacent vertices
//
unsigned int* GraphGetAdjacentsTo(const Graph* g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List* vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex* vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  unsigned int* adjacent =
      (unsigned int*)calloc(1 + numAdjVertices, sizeof(unsigned int));

  if (numAdjVertices > 0) {
    adjacent[0] = numAdjVertices;
    List* adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge* ePointer = ListGetCurrentItem(adjList);
      adjacent[i + 1] = ePointer->adjVertex;
    }
  }

  return adjacent;
}

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by the distances to the adjacent vertices
//
double* GraphGetDistancesToAdjacents(const Graph* g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List* vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex* vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  double* distance = (double*)calloc(1 + numAdjVertices, sizeof(double));

  if (numAdjVertices > 0) {
    distance[0] = numAdjVertices;
    List* adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge* ePointer = ListGetCurrentItem(adjList);
      distance[i + 1] = ePointer->weight;
    }
  }

  return distance;
}

//
// For a graph
//
unsigned int GraphGetVertexDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 0);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexOutDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexInDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->inDegree;
}

// Edges

static int _addEdge(Graph* g, unsigned int v, unsigned int w, double weight) {
  // Insert edge (v,w)
  struct _Edge* edge_v_w = (struct _Edge*)malloc(sizeof(struct _Edge));
  edge_v_w->adjVertex = w;
  edge_v_w->weight = weight;

  ListMove(g->verticesList, v);
  struct _Vertex* vertex_v = ListGetCurrentItem(g->verticesList);
  int result = ListInsert(vertex_v->edgesList, edge_v_w);

  if (result == -1) {
    // Insertion failed --- Destroy the allocated edge
    free(edge_v_w);
    return 0;
  }

  // Update
  g->numEdges++;
  vertex_v->outDegree++;

  ListMove(g->verticesList, w);
  struct _Vertex* vertex_w = ListGetCurrentItem(g->verticesList);
  // DIRECTED GRAPH --- Update the in-degree of vertex w
  if (g->isDigraph == 1) {
    vertex_w->inDegree++;
  }

  // If UNDIRECTED GRAPH
  if (g->isDigraph == 0) {
    // It is a BIDIRECTIONAL EDGE --- Insert edge (w,v)
    struct _Edge* edge_w_v = (struct _Edge*)malloc(sizeof(struct _Edge));
    edge_w_v->adjVertex = v;
    edge_w_v->weight = weight;

    result = ListInsert(vertex_w->edgesList, edge_w_v);

    if (result == -1) {
      // Insertion failed --- Destroy the allocated edge
      free(edge_w_v);

      // And remove the edge (v,w) that was inserted above
      ListSearch(vertex_v->edgesList, (void*)edge_v_w);
      ListRemoveCurrent(vertex_v->edgesList);

      // UNDO the updates
      g->numEdges--;
      vertex_v->outDegree--;
      vertex_w->inDegree--;

      return 0;
    } else {
      // g->numEdges++; // Do not count the same edge twice on a undirected
      // graph !!

      // Just update the outDegree of vertex w
      vertex_w->outDegree++;
    }
  }

  return 1;
}

int GraphAddEdge(Graph* g, unsigned int v, unsigned int w) {
  assert(g->isWeighted == 0);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, 1.0);
}

int GraphAddWeightedEdge(Graph* g, unsigned int v, unsigned int w,
                         double weight) {
  assert(g->isWeighted == 1);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, weight);
}

// CHECKING

int GraphCheckInvariants(const Graph* g) {
  assert(g != NULL);

  assert(g->isComplete == 0 || g->isComplete == 1);
  assert(g->isDigraph == 0 || g->isDigraph == 1);
  assert(g->isWeighted == 0 || g->isWeighted == 1);

  if (g->isComplete) {
    unsigned int n = g->numVertices;
    if (g->isDigraph) {
      assert(g->numEdges == n * (n - 1));
    } else {
      assert(g->numEdges == n * (n - 1) / 2);
    }
  }

  // Checking the vertices list
  ListTestInvariants(g->verticesList);
  assert((int)g->numVertices == ListGetSize(g->verticesList));

  // Checking the total number of edges
  unsigned int out_degree_total = 0;
  unsigned int in_degree_total = 0;

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    out_degree_total += v->outDegree;
    if (g->isDigraph) {
      in_degree_total += v->inDegree;
    }
  }

  if (g->isDigraph) {
    assert(in_degree_total == out_degree_total);
    assert(g->numEdges == out_degree_total);
  } else {
    // Unidrected graph
    assert(g->numEdges == out_degree_total / 2);
  }

  // For each vertex, checking its adjacency list
  ListMoveToHead(vertices);
  i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    List* edges = v->edgesList;
    ListTestInvariants(edges);
    assert((int)v->outDegree == ListGetSize(edges));
  }

  return 0;
}

// DISPLAYING on the console

void GraphDisplay(const Graph* g) {
  printf("---\n");
  if (g->isWeighted) {
    printf("Weighted ");
  }
  if (g->isComplete) {
    printf("COMPLETE ");
  }
  if (g->isDigraph) {
    printf("Digraph\n");
    printf("Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
  } else {
    printf("Graph\n");
    printf("Max Degree = %d\n", GraphGetMaxDegree(g));
  }
  printf("Vertices = %2d | Edges = %2d\n", g->numVertices, g->numEdges);

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    printf("%2d ->", i);
    struct _Vertex* v = ListGetCurrentItem(vertices);
    if (ListIsEmpty(v->edgesList)) {
      printf("\n");
    } else {
      List* edges = v->edgesList;
      int i = 0;
      ListMoveToHead(edges);
      for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
        struct _Edge* e = ListGetCurrentItem(edges);
        if (g->isWeighted) {
          printf("   %2d(%4.2f)", e->adjVertex, e->weight);
        } else {
          printf("   %2d", e->adjVertex);
        }
      }
      printf("\n");
      // Checking the invariants of the list of edges
      ListTestInvariants(edges);
    }
  }
  printf("---\n");
  // Checking the invariants of the list of vertices
  ListTestInvariants(vertices);
}

void GraphListAdjacents(const Graph* g, unsigned int v) {
  printf("---\n");

  unsigned int* array = GraphGetAdjacentsTo(g, v);

  printf("Vertex %d has %d adjacent vertices -> ", v, array[0]);

  for (unsigned int i = 1; i <= array[0]; i++) {
    printf("%d ", array[i]);
  }

  printf("\n");

  free(array);

  printf("---\n");
}

// Display the graph in DOT language.
// To draw the graph, you can use dot (from Graphviz) or paste result on:
//   https://dreampuf.github.io/GraphvizOnline
void GraphDisplayDOT(const Graph* g) {
  char* gtypes[] = {"graph", "digraph"};
  char* edgeops[] = {"--", "->"};
  char* gtype = gtypes[g->isDigraph];
  char* edgeop = edgeops[g->isDigraph];

  printf("// Paste in: https://dreampuf.github.io/GraphvizOnline\n");
  printf("%s {\n", gtype);
  printf("  // Vertices = %2d\n", g->numVertices);
  printf("  // Edges = %2d\n", g->numEdges);
  if (g->isDigraph) {
    printf("  // Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
  } else {
    printf("  // Max Degree = %d\n", GraphGetMaxDegree(g));
  }

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    printf("  %d;\n", i);
    struct _Vertex* v = ListGetCurrentItem(vertices);
    List* edges = v->edgesList;
    int k = 0;
    ListMoveToHead(edges);
    for (; k < ListGetSize(edges); ListMoveToNext(edges), k++) {
      struct _Edge* e = ListGetCurrentItem(edges);
      unsigned int j = e->adjVertex;
      if (g->isDigraph || i <= j) {  // for graphs, draw only 1 edge
        printf("  %d %s %d", i, edgeop, j);
        if (g->isWeighted) {
          printf(" [label=%4.2f]", e->weight);
        }
        printf(";\n");
      }
    }
  }
  printf("}\n");
}
