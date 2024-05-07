#include <stdio.h>
#include <limits.h>

#define V 4 // 图的顶点数

// 找到未被包含在最短路径树中并且具有最短距离的顶点
int minDistance(int dist[], int sptSet[]) {//dist[]: 保存从源到每个顶点的最短距离的数组。
                                           //sptSet[]: 如果顶点 i 包含在最短路径树中，则 sptSet[i] 为真。用于标记顶点是否被包含在最短路径树中。
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;//未被包含在最短路径树中并且具有最短距离的顶点的索引。
}

// 打印最短路径
void printSolution(int dist[]) {
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t %d\n", i, dist[i]);
}

// 实现 Dijkstra 算法
void dijkstra(int graph[V][V], int src) {
    int dist[V]; // 保存从源到每个顶点的最短距离
    int sptSet[V]; // 如果顶点 i 包含在最短路径树中，则 sptSet[i] 为真

    // 初始化所有距离为无穷大，sptSet 为假
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }

    // 源到自身的距离为 0
    dist[src] = 0;

    // 找到最短路径
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
        }
    }
    // 打印结果
    printSolution(dist);
}



