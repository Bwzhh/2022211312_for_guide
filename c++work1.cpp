#include <stdio.h>

// �������� - ��������������
void insertion_sort_by_reviews(int arr[][2], int n) {//��ά���飬�洢�˵ص����Ϣ��
//arr[i][0] ��ʾ�� i ���ص�����ƣ�arr[i][1] ��ʾ�� i ���ص������������
    int i, j, key_name, key_reviews;
    for (i = 1; i < n; i++) {
        key_name = arr[i][0];
        key_reviews = arr[i][1];
        j = i - 1;

        // �ƶ�Ԫ�أ�ֱ���ҵ����ʵ�λ�ò���
        while (j >= 0 && arr[j][1] < key_reviews) {
            arr[j+1][0] = arr[j][0];
            arr[j+1][1] = arr[j][1];
            j = j - 1;
        }
        arr[j+1][0] = key_name;
        arr[j+1][1] = key_reviews;
    }
}

// �������� - ���ȶ�����
void insertion_sort_by_popularity(int arr[][2], int n) {
    int i, j, key_name, key_popularity;
    for (i = 1; i < n; i++) {
        key_name = arr[i][0];
        key_popularity = arr[i][1]; // ʹ���ȶ������������
        j = i - 1;

        // �ƶ�Ԫ�أ�ֱ���ҵ����ʵ�λ�ò���
        while (j >= 0 && arr[j][1] < key_popularity) { // ��Ϊʹ���ȶȽ��бȽ�
            arr[j+1][0] = arr[j][0];
            arr[j+1][1] = arr[j][1];
            j = j - 1;
        }
        arr[j+1][0] = key_name;
        arr[j+1][1] = key_popularity;
    }
}


// ѡ������ - ��������������
void selection_sort_by_reviews(int arr[][2], int n) {//ѡ������ֻ��ѡ��ǰʮ������ǰʮ��û��˳��
    int i, j, max_idx;
    for (i = 0; i < 10 && i < n - 1; i++) {
        max_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j][1] > arr[max_idx][1]) {
                max_idx = j;
            }
        }
        // Swap
        int temp_name = arr[i][0];
        int temp_reviews = arr[i][1];
        arr[i][0] = arr[max_idx][0];
        arr[i][1] = arr[max_idx][1];
        arr[max_idx][0] = temp_name;
        arr[max_idx][1] = temp_reviews;
    }
}


// ѡ������ - ���ȶ�����
void selection_sort_by_popularity(int arr[][2], int n) {
    int i, j, max_idx;
    for (i = 0; i < 10 && i < n - 1; i++) {
        max_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j][1] > arr[max_idx][1]) {
                max_idx = j;
            }
        }
        // Swap
        int temp_name = arr[i][0];
        int temp_popularity = arr[i][1];
        arr[i][0] = arr[max_idx][0];
        arr[i][1] = arr[max_idx][1];
        arr[max_idx][0] = temp_name;
        arr[max_idx][1] = temp_popularity;
    }
}

//ͨ��������������ǰʮ����ѧ�ص�






