#include <stdio.h>
#include <limits.h>

#define V 4 // ͼ�Ķ�����

// ����ӵ���ȶ���ʻʱ���Ӱ��
float calculate_time(float distance, float congestion) {
    // ���������ٶ�Ϊ 10 km/h
    float ideal_speed = 10.0;
    // ��ʵ�ٶ� = ӵ���� * �����ٶ�
    float real_speed = congestion * ideal_speed;
    // ʱ�� = ���� / ��ʵ�ٶ�
    float time = distance / real_speed;
    return time;
}

// ʵ�� Floyd-Warshall �㷨�����ǵ�·ӵ����
void floydWarshall(float graph[V][V]) {
    float dist[V][V]; // �����ÿ�����㵽ÿ����������ʱ��

    // ��ʼ���������
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            dist[i][j] = graph[i][j];

    // ͨ���м䶥����¾������
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                // ������� k �ڴ� i �� j ��·���ϣ�����¾������
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    // ����ӵ���ȶ���ʻʱ���Ӱ��
                    float time = calculate_time(dist[i][k], dist[i][k]);
                    if (dist[i][k] + time < dist[i][j])
                        dist[i][j] = dist[i][k] + time;
                }
            }
        }
    }

    // ��ӡ���
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

