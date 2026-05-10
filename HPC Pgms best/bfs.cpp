#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 100
#define INF 999999

int graph[MAX_NODES][MAX_NODES]; // Adjacency matrix
int visited[MAX_NODES];
int distance[MAX_NODES];

void parallel_bfs(int source, int num_nodes) {
    #pragma omp parallel for
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = 0;
        distance[i] = INF;
    }

    visited[source] = 1;
    distance[source] = 0;

    int queue[MAX_NODES];
    int front = 0, rear = 0;
    queue[rear++] = source;

    while (front < rear) {
        int u = queue[front++];
        #pragma omp parallel for
        for (int v = 0; v < num_nodes; v++) {
            if (graph[u][v] && !visited[v]) {
                visited[v] = 1;
                distance[v] = distance[u] + 1;
                queue[rear++] = v;
            }
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
    printf("Enter the source node: ");
    scanf("%d", &source);

    parallel_bfs(source, num_nodes);

    printf("Shortest distances from node %d:\n", source);
    for (int i = 0; i < num_nodes; i++) {
        printf("Node %d: Distance %d\n", i, distance[i]);
    }

    return 0;
}

