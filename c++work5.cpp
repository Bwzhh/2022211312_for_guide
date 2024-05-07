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

// 实现 Floyd-Warshall 算法，考虑道路拥挤度
void floydWarshall(float graph[V][V]) {
    float dist[V][V]; // 保存从每个顶点到每个顶点的最短时间

    // 初始化距离矩阵
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    // 通过中间顶点更新距离矩阵
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                // 如果顶点 k 在从 i 到 j 的路径上，则更新距离矩阵
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    // 计算拥挤度对行驶时间的影响
                    float time = calculate_time(dist[i][k], dist[i][k]);
                    if (dist[i][k] + time < dist[i][j])
                        dist[i][j] = dist[i][k] + time;
                }
            }
        }
    }

    // 打印结果
    printf("Shortest time between every pair of vertices:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INT_MAX)
                printf("%7s", "INF");
            else
                printf ("%7.2f", dist[i][j]);
        }
        printf("\n");
    }
}

