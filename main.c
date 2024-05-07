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
    printf("����������ص㣺");
    fgets(diary->location, sizeof(diary->location), stdin);
    printf("������������᣺");
    fgets(diary->experience, sizeof(diary->experience), stdin);
}

void saveDiary(struct Diary *diary) {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("�޷����ռ��ļ���\n");
        exit(1);
    }
    fprintf(file, "%s%s", diary->location, diary->experience);
    fclose(file);
    printf("�ռ��ѱ��棡\n");
}

void readDiary() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("�ռ��ļ�Ϊ�ջ򲻴��ڣ�\n");
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
            printf("�޷������ռ��ļ���\n");
            exit(1);
        }
        fclose(createFile);
    } else {
        fclose(checkFile);
    }

    struct Diary myDiary;
    int choice = 0;

    printf("��ӭʹ���ռǹ���ϵͳ��\n");

    while (choice != 3) {
        printf("\n��ѡ�������\n");
        printf("1. �ϴ��ռ�\n");
        printf("2. �鿴�ռ�\n");
        printf("3. �˳�\n");
        printf("����������ѡ��");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1:
                uploadDiary(&myDiary);
                saveDiary(&myDiary);
                break;
            case 2:
                printf("---- �����������ռ� ----\n");
                readDiary();
                break;
            case 3:
                printf("ллʹ�ã��ټ���\n");
                break;
            default:
                printf("��Ч��ѡ�����������롣\n");
        }
    }

    return 0;
}
