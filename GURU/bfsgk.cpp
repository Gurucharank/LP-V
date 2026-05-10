#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 100
#define INF 999999

int graph[MAX_NODES][MAX_NODES];
int visited[MAX_NODES];
int dist[MAX_NODES];
int queue[MAX_NODES];
int front = 0, rear = 0;

void parallel_bfs(int source, int num_nodes) {
    #pragma omp parallel for
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = 0;
        dist[i] = INF;
    }

    visited[source] = 1;
    dist[source] = 0;
    
    #pragma omp critical
    queue[rear++] = source;

    while (front < rear) {
        int u;
        
        #pragma omp critical
        {
            if (front < rear) {
                u = queue[front++];
            } else {
                u = -1;
            }
        }

        if (u == -1) break;

        #pragma omp parallel for
        for (int v = 0; v < num_nodes; v++) {
            if (graph[u][v] && !visited[v]) {
                #pragma omp critical
                {
                    if (!visited[v]) {
                        visited[v] = 1;
                        dist[v] = dist[u] + 1;
                        queue[rear++] = v;
                    }
                }
            }
        }
    }
}

int main() {
    int num_nodes, source;
    printf("--- Parallel BFS ---\n");
    printf("Enter number of nodes: ");
    scanf("%d", &num_nodes);

    printf("Enter adjacency matrix:\n");
    for (int i = 0; i < num_nodes; i++)
        for (int j = 0; j < num_nodes; j++)
            scanf("%d", &graph[i][j]);

    printf("Enter source node (0 to %d): ", num_nodes - 1);
    scanf("%d", &source);

    parallel_bfs(source, num_nodes);

    printf("\nNode distances from source %d:\n", source);
    for (int i = 0; i < num_nodes; i++) {
        if (dist[i] == INF) printf("Node %d: Unreachable\n", i);
        else printf("Node %d: %d\n", i, dist[i]);
    }

    return 0;
}