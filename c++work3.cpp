#include <stdio.h>
#include <limits.h>

#define V 4 // 图的顶点数

// 实现 Floyd-Warshall 算法
void floydWarshall(int graph[V][V]) {
    int dist[V][V];

    // 初始化距离矩阵
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    // 通过顶点 k 来更新距离矩阵
    for (int k = 0; k < V; k++) {
        // 选择所有顶点对 (i, j)
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                // 如果顶点 k 在从 i 到 j 的路径上，则更新距离矩阵
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // 打印结果
    printf ("Shortest distances between every pair of vertices:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INT_MAX)
                printf("%7s", "INF");
            else
                printf ("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

