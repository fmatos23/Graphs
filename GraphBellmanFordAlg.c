//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name : Francisco Matos
// Student Number : 113726
// Student Name : Luís Rodrigues
// Student Number : 114560

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "instrumentation.h"
#include "SortedList.h" 
#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,unsigned int startVertex) {
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);

  GraphBellmanFordAlg* result = (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  // Given graph and start vertex for the shortest-paths
  result->graph = g;
  result->startVertex = startVertex;

  unsigned int numVertices = GraphGetNumVertices(g);

  //
  // TO BE COMPLETED !!
  //
  // CREATE AND INITIALIZE
  // result->marked
  // result->distance
  // result->predecessor
  //

  // Mark all vertices as not yet visited, i.e., ZERO
  
  // No vertex has (yet) a (valid) predecessor
  
  // No vertex has (yet) a (valid) distance to the start vertex
  
  // THE ALGORTIHM TO BUILD THE SHORTEST-PATHS TREE
  
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  result->distance = (int*)malloc(numVertices * sizeof(int));
  result->predecessor = (int*)malloc(numVertices * sizeof(int));
  assert(result->marked != NULL && result->distance != NULL && result->predecessor != NULL);

  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = INT_MAX;
    result->predecessor[i] = -1;
  }
  result->distance[startVertex] = 0;

  // Measure execution time
  double startTime = cpu_time();

  // Bellman-Ford Algorithm
  for (unsigned int i = 1; i < numVertices; i++) {
    for (unsigned int u = 0; u < numVertices; u++) {
      unsigned int* adjacents = GraphGetAdjacentsTo(g, u);
      unsigned int numAdjacents = adjacents[0];

      for (unsigned int j = 1; j <= numAdjacents; j++) {
        unsigned int v = adjacents[j];
        if (result->distance[u] != INT_MAX && result->distance[u] + 1 < result->distance[v]) {
          result->distance[v] = result->distance[u] + 1;
          result->predecessor[v] = u;
          result->marked[v] = 1;
        }
      }
      free(adjacents);  // Free adjacency list
    }
  }

  double endTime = cpu_time();
  printf("Execution Time for Bellman-Ford (start vertex %u): %.6f seconds\n", startVertex, endTime - startTime);

  return result;
}


// Verifica se o vértice `v` foi alcançado pelo algoritmo Bellman-Ford
int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
    assert(p != NULL);
    assert(v < GraphGetNumVertices(p->graph));
    return p->distance[v] != INT_MAX; // Retorna 1 se alcançado, 0 caso contrário
}


Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {  // Se o vértice não foi alcançado
    return s;
  }

  // Caminho do vértice v até o vértice inicial
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);  // Adiciona o vértice inicial

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

void testBellmanFordInstrumentation(Graph* g) {
    InstrName[0] = "Relaxations";
    InstrName[1] = "EdgeAccess";
    InstrCalibrate();

    InstrReset();  // Reseta os contadores
    unsigned int V = GraphGetNumVertices(g);

    for (unsigned int i = 0; i < V; i++) {
        GraphBellmanFordAlg* bfAlg = GraphBellmanFordAlgExecute(g, i);

        // Contar relaxamentos e acessos às arestas
        InstrCount[0] += (V - 1);  // Relaxações
        InstrCount[1] += GraphGetNumEdges(g);  // Acessos às arestas

        GraphBellmanFordAlgDestroy(&bfAlg);
    }

    InstrPrint();  // Imprime os resultados
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
    assert(p != NULL);
    assert(v < GraphGetNumVertices(p->graph));
    return p->distance[v];  // Retorna a distância armazenada para o vértice `v`
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // The paths tree is a digraph, with no edge weights
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use the predecessors array to add the tree edges
  for (unsigned int w = 0; w < num_vertices; w++) {
    // Vertex w has a predecessor vertex v?
    int v = p->predecessor[w];
    if (v != -1) {
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Display the tree in the DOT format
  GraphDisplayDOT(paths_tree);

  // Housekeeping
  GraphDestroy(&paths_tree);
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(p != NULL && *p != NULL);

  GraphBellmanFordAlg* aux = *p;

  // Libertar a memória alocada
  free(aux->marked);
  free(aux->distance);
  free(aux->predecessor);

  free(aux);  // Libertar a estrutura principal
  *p = NULL;  // Apontar para NULL para evitar referências pendentes
}
