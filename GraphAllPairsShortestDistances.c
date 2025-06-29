//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphAllPairsShortestDistances
//

// Student Name : Francisco Matos 
// Student Number : 113726
// Student Name : Luís Rodrigues
// Student Number : 114560

/*** COMPLETE THE GraphAllPairsShortestDistancesExecute FUNCTION ***/

#include "GraphAllPairsShortestDistances.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"

struct _GraphAllPairsShortestDistances {
  int** distance;  // The 2D matrix storing the all-pairs shortest distances
                   // It is stored as an array of pointers to 1D rows
                   // Idea: an INDEFINITE distance value is stored as -1
  Graph* graph;
};

// Allocate memory and initialize the distance matrix
// Compute the distances between vertices by running the Bellman-Ford algorithm
GraphAllPairsShortestDistances* GraphAllPairsShortestDistancesExecute(Graph* g) {
  
  assert(g != NULL);
  // COMPLETE THE CODE
  unsigned int V = GraphGetNumVertices(g);
  GraphAllPairsShortestDistances* result = (GraphAllPairsShortestDistances*)malloc(sizeof(GraphAllPairsShortestDistances));
  result->graph = g;

  // Allocate the distance matrix
  result->distance = (int**)malloc(V * sizeof(int*));
  for (unsigned int i = 0; i < V; i++) {
    result->distance[i] = (int*)malloc(V * sizeof(int));
    for (unsigned int j = 0; j < V; j++) {
      result->distance[i][j] = (i == j) ? 0 : INT_MAX;  // Distance to itself is 0
    }
  }

  // Compute shortest paths using Bellman-Ford
  for (unsigned int i = 0; i < V; i++) {
    // Execute Bellman-Ford for vertex `i`
    GraphBellmanFordAlg* bfAlg = GraphBellmanFordAlgExecute(g, i);

    for (unsigned int j = 0; j < V; j++) {
      if (GraphBellmanFordAlgReached(bfAlg, j)) {
        result->distance[i][j] = GraphBellmanFordAlgDistance(bfAlg, j);
      } else {
        result->distance[i][j] = INT_MAX; // Vertex `j` is not reachable from `i`
      }
    }

    // Destroy Bellman-Ford results
    GraphBellmanFordAlgDestroy(&bfAlg);
  }

  return result;

}



void GraphAllPairsShortestDistancesDestroy(GraphAllPairsShortestDistances** p) {
  assert(*p != NULL);

  GraphAllPairsShortestDistances* aux = *p;
  unsigned int numVertices = GraphGetNumVertices(aux->graph);

  for (unsigned int i = 0; i < numVertices; i++) {
    free(aux->distance[i]);
  }

  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the result

int GraphGetDistanceVW(const GraphAllPairsShortestDistances* p, unsigned int v,
                       unsigned int w) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(w < GraphGetNumVertices(p->graph));

  return p->distance[v][w];
}

// DISPLAYING on the console

void GraphAllPairsShortestDistancesPrint(
    const GraphAllPairsShortestDistances* p) {
  assert(p != NULL);

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Graph distance matrix - %u vertices\n", numVertices);

  for (unsigned int i = 0; i < numVertices; i++) {
    for (unsigned int j = 0; j < numVertices; j++) {
      int distanceIJ = p->distance[i][j];
      if (distanceIJ == -1) {
        // INFINITY - j was not reached from i
        printf(" INF");
      } else {
        printf(" %3d", distanceIJ);
      }
    }
    printf("\n");
  }
}
