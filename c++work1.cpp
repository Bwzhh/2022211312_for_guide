#include <stdio.h>

// 插入排序 - 按评价数量排序
void insertion_sort_by_reviews(int arr[][2], int n) {//二维数组，存储了地点的信息。
//arr[i][0] 表示第 i 个地点的名称，arr[i][1] 表示第 i 个地点的评价数量。
    int i, j, key_name, key_reviews;
    for (i = 1; i < n; i++) {
        key_name = arr[i][0];
        key_reviews = arr[i][1];
        j = i - 1;

        // 移动元素，直到找到合适的位置插入
        while (j >= 0 && arr[j][1] < key_reviews) {
            arr[j+1][0] = arr[j][0];
            arr[j+1][1] = arr[j][1];
            j = j - 1;
        }
        arr[j+1][0] = key_name;
        arr[j+1][1] = key_reviews;
    }
}

// 插入排序 - 按热度排序
void insertion_sort_by_popularity(int arr[][2], int n) {
    int i, j, key_name, key_popularity;
    for (i = 1; i < n; i++) {
        key_name = arr[i][0];
        key_popularity = arr[i][1]; // 使用热度替代评价数量
        j = i - 1;

        // 移动元素，直到找到合适的位置插入
        while (j >= 0 && arr[j][1] < key_popularity) { // 改为使用热度进行比较
            arr[j+1][0] = arr[j][0];
            arr[j+1][1] = arr[j][1];
            j = j - 1;
        }
        arr[j+1][0] = key_name;
        arr[j+1][1] = key_popularity;
    }
}


// 选择排序 - 按评价数量排序
void selection_sort_by_reviews(int arr[][2], int n) {//选择排序只能选出前十，但是前十个没有顺序
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


// 选择排序 - 按热度排序
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

//通过控制输出来输出前十个游学地点






