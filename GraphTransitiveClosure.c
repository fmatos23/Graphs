//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name :   Francisco Matos
// Student Number : 113726
// Student Name : Luís Rodrigues
// Student Number: 114560

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"
#include "GraphBellmanFordAlg.h"  // Para Bellman-Ford
#include "instrumentation.h"      // Para cpu_time
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
Graph* GraphComputeTransitiveClosure(Graph* g) {
    assert(g != NULL);                // Ensure the graph is not NULL
    assert(GraphIsDigraph(g));        // Ensure the graph is directed
    assert(GraphIsWeighted(g) == 0);  // Ensure the graph has no weights

    int numVertices = GraphGetNumVertices(g);

    // Create a new graph to store the transitive closure
    Graph* transitiveClosure = GraphCreate(numVertices, 1, 0); // 1 for directed, 0 for unweighted

    // Measure execution time
    double startTime = cpu_time();

    // For each vertex, calculate reachable vertices
    for (int u = 0; u < numVertices; u++) {
        // Use Bellman-Ford to find all reachable vertices from u
        GraphBellmanFordAlg* bfAlg = GraphBellmanFordAlgExecute(g, u);
        for (int v = 0; v < numVertices; v++) {
            // Add an edge u -> v if v is reachable from u and not a loop
            if (u != v && GraphBellmanFordAlgReached(bfAlg, v)) {
                GraphAddEdge(transitiveClosure, u, v);
            }
        }
        // Destroy the Bellman-Ford algorithm instance to free memory
        GraphBellmanFordAlgDestroy(&bfAlg);
    }

    // Capture end time after computation
    double endTime = cpu_time();

    // Print execution time
    printf("Tempo de execução do Transitive Closure: %.6f segundos\n", endTime - startTime);

    return transitiveClosure;
}
