#include <stdio.h>
#include <limits.h>

#define V 4 // ͼ�Ķ�����

// �ҵ�δ�����������·�����в��Ҿ�����̾���Ķ���
int minDistance(int dist[], int sptSet[]) {//dist[]: �����Դ��ÿ���������̾�������顣
                                           //sptSet[]: ������� i ���������·�����У��� sptSet[i] Ϊ�档���ڱ�Ƕ����Ƿ񱻰��������·�����С�
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;//δ�����������·�����в��Ҿ�����̾���Ķ����������
}

// ��ӡ���·��
void printSolution(int dist[]) {
    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t %d\n", i, dist[i]);
}

// ʵ�� Dijkstra �㷨
void dijkstra(int graph[V][V], int src) {
    int dist[V]; // �����Դ��ÿ���������̾���
    int sptSet[V]; // ������� i ���������·�����У��� sptSet[i] Ϊ��

    // ��ʼ�����о���Ϊ�����sptSet Ϊ��
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
    }

    // Դ������ľ���Ϊ 0
    dist[src] = 0;

    // �ҵ����·��
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
        }
    }
    // ��ӡ���
    printSolution(dist);
}



