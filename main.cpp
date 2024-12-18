#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <locale.h>

int** createG(int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                G[i][j] = 0; // ��� ����� �� ����� ���� ����� 0
            }
            else {
                G[i][j] = rand() % 10; // ��������� ��� �� 1 �� 9
            }
            G[j][i] = G[i][j]; // ���� �����������������
        }
    }
    return G;
}

int** createGManual(int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
    }

    printf("������� ���� ���� (0 ���� ����� ���):\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                G[i][j] = 0; // ��� ����� �� ����� ���� ����� 0
            }
            else {
                printf("��� ����� %d-%d: ", i, j);
                scanf("%d", &G[i][j]);
            }
            G[j][i] = G[i][j]; // ���� �����������������
        }
    }
    return G;
}

void printG(int** G, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int minKey(int key[], int mstSet[], int size) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < size; v++) 
    {
        if (mstSet[v] == 0 && key[v] < min)//���� ������� �� ���� �������� � ��� ��������������� ������ ����������(��� ������������� � ������������ � min) 
        {
            min = key[v];//�������� � ��� ���������
            min_index = v;//����� ������ ������, ��������������� ���� ������� � ���������� �����
        }
    }
    return min_index;//������� ������ ������� � ����������� �����
}

int** primMST(int** graph, int size) {
    int* parent = (int*)malloc(size * sizeof(int)); // ������ �������� ���������
    int* key = (int*)malloc(size * sizeof(int)); // ������ ����������� �����
    int* mstSet = (int*)malloc(size * sizeof(int)); // ������ ���������� (����������) ������
    int** mstGraph = (int**)malloc(size * sizeof(int*)); // ������������ ������� ���������

    // �������������� ������� ��������� ��� MST
    for (int i = 0; i < size; i++) {
        mstGraph[i] = (int*)calloc(size, sizeof(int)); // �������������� ������
    }

    for (int i = 0; i < size; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    key[0] = 0; // ������������ 0 ��� ������� ��������, ��� ��� ���������� ����� 0
    parent[0] = -1; // ������������ -1, �.�. � ������� �������� ��� ���������

    int visitedCount = 0; // ������� ���������� ������

    while (visitedCount < size) { // ����������, ���� �� ������� ��� �������
        int u = minKey(key, mstSet, size); // u - ������ � ������ � ����������� ����� 
        mstSet[u] = 1; // ������������ 1, ��� ��� �� �������� ��� �������
        visitedCount++; // ����������� ������� ���������� ������

        for (int v = 0; v < size; v++) {
            if (graph[u][v] && mstSet[v] == 0 && graph[u][v] < key[v]) { // ���� � �������� ����� �� ������������ ������� ������� � ���������� ������� ������ � ��� ������� �� ��������������� ����� (MSTset) � �������� � ���� ��������������� ������ ������ ������������ ����
                parent[v] = u; // �������� ���������� �������� ����� ������������, �������� ������������ �����
                key[v] = graph[u][v]; // �� ���������� ������� ����� ����� ������� ��� �����
            }
        }
    }

    // ���������� ������� ��������� ��� MST
    for (int i = 1; i < size; i++) {
        mstGraph[parent[i]][i] = graph[parent[i]][i];
        mstGraph[i][parent[i]] = graph[parent[i]][i];
    }

    free(parent);
    free(key);
    free(mstSet);

    return mstGraph; // ���������� ������� ��������� ��� MST
}

void saveToFile(int** graph, int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("������ �������� ����� ��� ������.\n");
        return;
    }

    fprintf(file, "%d\n", size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d ", graph[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("���������� ��������� � ���� %s.\n", filename);
}

int main() {
    srand(time(NULL)); // ������������� ���������� ��������� �����
    setlocale(LC_ALL, "RU");

    int size = 0;
    char option;
    int** lastGraph = NULL; // ��� �������� ���������� ���������� �����
    int** lastMST = NULL; // ��� �������� ���������� ������������ ��������� ������

    while (1) {
        printf("����:\n");
        printf("1. ������ ���-�� ������ �����\n");
        printf("2. ������� ���� ��������\n");
        printf("3. ������� ���� �������\n");
        printf("4. ��������� ����������� �������� ������\n");
        printf("5. ��������� ���������� � ����\n");
        printf("6. �����\n");
        printf("�������� �����: ");
        scanf(" %c", &option);
        {
            switch (option) {
            case '1':
                printf("������� ���-�� ������ �����: ");
                scanf("%d", &size);
                break;

            case '2': {
                if (size <= 0) {
                    printf("������� ������� ���-�� ������ �����.\n");
                    break;
                }
                lastGraph = createG(size);
                printf("��������������� ����:\n");
                printG(lastGraph, size);
                lastMST = primMST(lastGraph, size);
                break;
            }

            case '3': {
                if (size <= 0) {
                    printf("������� ������� ���-�� ������ �����.\n");
                    break;
                }
                lastGraph = createGManual(size);
                printf("��������� ����:\n");
                printG(lastGraph, size);
                lastMST = primMST(lastGraph, size);
                break;
            }

            case '4': {
                if (lastGraph == NULL) {
                    printf("������� �������� ����.\n");
                    break;
                }
                lastMST = primMST(lastGraph, size);
                printf("������� ��������� ������������ ��������� ������:\n");
                printG(lastMST, size);
                break;
            }

            case '5': {
                if (lastGraph == NULL) {
                    printf("������� �������� ����.\n");
                    break;
                }
                printf("��������� � ���� (������� ��� �����): ");
                char filename[100];
                scanf("%s", filename);
                saveToFile(lastMST, size, filename);
                break;
            }

            case '6':
                // ������������ ������ ����� �������
                if (lastGraph != NULL) {
                    for (int i = 0; i < size; i++) {
                        free(lastGraph[i]);
                    }
                    free(lastGraph);
                }
                if (lastMST != NULL) {
                    for (int i = 0; i < size; i++) {
                        free(lastMST[i]);
                    }
                    free(lastMST);
                }
                exit(0);

            default:
                printf("�������� �����, ���������� �����.\n");
            }
        }
    }
    return 0;
}
