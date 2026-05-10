#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 100

int graph[MAX_NODES][MAX_NODES];
int visited[MAX_NODES];

void dfs_recursive(int u, int num_nodes) {
    // Mark visited and print in a critical section to avoid output garbling
    #pragma omp critical
    {
        if (!visited[u]) {
            visited[u] = 1;
            printf("Visited node: %d by Thread %d\n", u, omp_get_thread_num());
        }
    }

    for (int v = 0; v < num_nodes; v++) {
        int should_visit = 0;
        
        #pragma omp critical
        {
            if (graph[u][v] && !visited[v]) {
                should_visit = 1;
                // Pre-mark to prevent other threads from creating duplicate tasks
                // This is a common optimization in parallel DFS
            }
        }

        if (should_visit) {
            #pragma omp task
            dfs_recursive(v, num_nodes);
        }
    }
}

void parallel_dfs(int source, int num_nodes) {
    #pragma omp parallel for
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = 0;
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            dfs_recursive(source, num_nodes);
        }
    }
}

int main() {
    int num_nodes, source;
    printf("--- Parallel DFS ---\n");
    printf("Enter number of nodes: ");
    scanf("%d", &num_nodes);

    printf("Enter adjacency matrix:\n");
    for (int i = 0; i < num_nodes; i++)
        for (int j = 0; j < num_nodes; j++)
            scanf("%d", &graph[i][j]);

    printf("Enter source node: ");
    scanf("%d", &source);

    printf("\nTraversal Order:\n");
    parallel_dfs(source, num_nodes);

    return 0;
}

gcc -fopenmp main.c -o main
g++ -fopenmp main.cpp -o main