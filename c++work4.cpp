#include <stdio.h>
#include <limits.h>

#define V 4 // 图的顶点数

// 计算拥挤度对行驶时间的影响
float calculate_time(float distance, float congestion) {
    // 假设理想速度为 10 km/h
    float ideal_speed = 10.0;
    // 真实速度 = 拥挤度 * 理想速度
    float real_speed = congestion * ideal_speed;
    // 时间 = 距离 / 真实速度
    float time = distance / real_speed;
    return time;
}

// 找到未被包含在最短路径树中并且具有最短时间的顶点
int minTimeVertex(float dist[], int sptSet[]) {
    float min = INT_MAX;
    int min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// 实现 Dijkstra 算法，考虑道路拥挤度
void dijkstra(float graph[V][V], int src) {
    float dist[V]; // 保存从源到每个顶点的最短时间
    int sptSet[V]; // 如果顶点 i 包含在最短路径树中，则 sptSet[i] 为真

    // 初始化所有时间为无穷大，sptSet 为假
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }

    // 源到自身的时间为 0
    dist[src] = 0;

    // 找到最短时间路径
    for (int count = 0; count < V - 1; count++) {
        int u = minTimeVertex(dist, sptSet);
        sptSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX) {
                // 计算拥挤度对行驶时间的影响
                float time = calculate_time(graph[u][v], graph[u][v]);//将道路距离和拥挤度信息都存储到图的邻接矩阵中
                if (dist[u] + time < dist[v])
                    dist[v] = dist[u] + time;
            }
        }
    }

    // 打印结果
    printf("Vertex \t Time from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t %.2f\n", i, dist[i]);
}

