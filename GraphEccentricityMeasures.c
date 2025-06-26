//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name : Francisco Matos
// Student Number : 113726
// Student Name : Luís Rodrigues
// Student Number : 114560

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL);

  // COMPLETE THE CODE
  // CREATE AUXILIARY (static) FUNCTIONS, IF USEFUL
  // Graph radius --- the smallest vertex eccentricity value
  // Graph diameter --- the largest vertex eccentricity value
  // Do not forget that -1 represents an IDEFINITE value

  // Computing the set of central vertices
  // Allocate the central vertices array : number of central vertices + 1
  // Fill in the central vertices array

  
  unsigned int V = GraphGetNumVertices(g);
  GraphEccentricityMeasures* measures = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  measures->graph = g;
  measures->eccentricity = (int*)malloc(V * sizeof(int));

  // Initialize eccentricity values to -1 (indefinite)
  for (unsigned int i = 0; i < V; i++) {
    measures->eccentricity[i] = -1;
  }

  // Use All-Pairs Shortest Distances to calculate eccentricity
  GraphAllPairsShortestDistances* apsd = GraphAllPairsShortestDistancesExecute(g);

  int graphRadius = INT_MAX;
  int graphDiameter = INT_MIN;

  for (unsigned int i = 0; i < V; i++) {
    int maxDistance = INT_MIN;
    for (unsigned int j = 0; j < V; j++) {
      int distance = GraphGetDistanceVW(apsd, i, j);
      if (distance != INT_MAX && distance > maxDistance) {
        maxDistance = distance;
      }
    }
    measures->eccentricity[i] = maxDistance;
    if (maxDistance < graphRadius) {
      graphRadius = maxDistance;
    }
    if (maxDistance > graphDiameter) {
      graphDiameter = maxDistance;
    }
  }

  measures->graphRadius = graphRadius;
  measures->graphDiameter = graphDiameter;

  // Compute central vertices
  unsigned int centralCount = 0;
  for (unsigned int i = 0; i < V; i++) {
    if (measures->eccentricity[i] == graphRadius) {
      centralCount++;
    }
  }

  measures->centralVertices = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
  measures->centralVertices[0] = centralCount;
  unsigned int index = 1;
  for (unsigned int i = 0; i < V; i++) {
    if (measures->eccentricity[i] == graphRadius) {
      measures->centralVertices[index++] = i;
    }
  }

  GraphAllPairsShortestDistancesDestroy(&apsd);

  return measures;
}

void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL);
  assert(p->centralVertices != NULL);

  unsigned int numCentralVertices = p->centralVertices[0];
  unsigned int* centralVerticesCopy = (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
  if (centralVerticesCopy == NULL) {
    return NULL; // Memory allocation failed
  }

  for (unsigned int i = 0; i <= numCentralVertices; i++) {
    centralVerticesCopy[i] = p->centralVertices[i];
  }

  return centralVerticesCopy;
}

// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  // COMPLETE THE CODE
  assert(p != NULL);

  // Print the graph radius
  printf("Graph Radius: %d\n", GraphGetRadius(p));

  // Print the graph diameter
  printf("Graph Diameter: %d\n", GraphGetDiameter(p));

  // Print the vertex eccentricity values
  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Vertex Eccentricity Values:\n");
  for (unsigned int v = 0; v < numVertices; v++) {
    printf("Vertex %u: %d\n", v, GraphGetVertexEccentricity(p, v));
  }

  // Print the set of central vertices
  unsigned int* centralVertices = GraphGetCentralVertices(p);
  if (centralVertices != NULL) {
    unsigned int numCentralVertices = centralVertices[0];
    printf("Central Vertices:\n");
    for (unsigned int i = 1; i <= numCentralVertices; i++) {
      printf("Vertex %u\n", centralVertices[i]);
    }
    free(centralVertices);
  } else {
    printf("No central vertices found.\n");
  }
}
