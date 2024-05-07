#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1000
#define FILE_NAME "diary.txt"

struct Diary {
    char location[MAX_LEN];
    char experience[MAX_LEN];
};

void uploadDiary(struct Diary *diary) {
    printf("请输入游玩地点：");
    fgets(diary->location, sizeof(diary->location), stdin);
    printf("请输入游玩体会：");
    fgets(diary->experience, sizeof(diary->experience), stdin);
}

void saveDiary(struct Diary *diary) {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("无法打开日记文件！\n");
        exit(1);
    }
    fprintf(file, "%s%s", diary->location, diary->experience);
    fclose(file);
    printf("日记已保存！\n");
}

void readDiary() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("日记文件为空或不存在！\n");
        return;
    }
    char buffer[MAX_LEN];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);
}

int main() {
    // Check if diary file exists, if not, create an empty one
    FILE *checkFile = fopen(FILE_NAME, "r");
    if (checkFile == NULL) {
        FILE *createFile = fopen(FILE_NAME, "w");
        if (createFile == NULL) {
            printf("无法创建日记文件！\n");
            exit(1);
        }
        fclose(createFile);
    } else {
        fclose(checkFile);
    }

    struct Diary myDiary;
    int choice = 0;

    printf("欢迎使用日记管理系统！\n");

    while (choice != 3) {
        printf("\n请选择操作：\n");
        printf("1. 上传日记\n");
        printf("2. 查看日记\n");
        printf("3. 退出\n");
        printf("请输入数字选择：");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1:
                uploadDiary(&myDiary);
                saveDiary(&myDiary);
                break;
            case 2:
                printf("---- 以下是您的日记 ----\n");
                readDiary();
                break;
            case 3:
                printf("谢谢使用，再见！\n");
                break;
            default:
                printf("无效的选择，请重新输入。\n");
        }
    }

    return 0;
}
