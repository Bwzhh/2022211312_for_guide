#include <stdio.h>
#include <limits.h>

#define V 4 // ͼ�Ķ�����

// ʵ�� Floyd-Warshall �㷨
void floydWarshall(int graph[V][V]) {
    int dist[V][V];

    // ��ʼ���������
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    // ͨ������ k �����¾������
    for (int k = 0; k < V; k++) {
        // ѡ�����ж���� (i, j)
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                // ������� k �ڴ� i �� j ��·���ϣ�����¾������
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // ��ӡ���
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

