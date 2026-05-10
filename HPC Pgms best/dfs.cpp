#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 100

int graph[MAX_NODES][MAX_NODES]; // Adjacency matrix
int visited[MAX_NODES];

void parallel_dfs(int node, int num_nodes) {
    visited[node] = 1;
    printf("Node visited: %d\n", node);

    #pragma omp parallel for
    for (int i = 0; i < num_nodes; i++) {
        if (graph[node][i] && !visited[i]) {
            parallel_dfs(i, num_nodes);
        }
    }
}

int main() {
    int num_nodes, source;
    printf("Enter the number of nodes: ");
    scanf("%d", &num_nodes);
    printf("Enter the adjacency matrix:\n");
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = 0;
    }

    printf("Enter the source node: ");
    scanf("%d", &source);

    parallel_dfs(source, num_nodes);

    return 0;
}

