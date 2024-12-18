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
                G[i][j] = 0; // Вес ребра до самой себя равен 0
            }
            else {
                G[i][j] = rand() % 10; // Случайный вес от 1 до 9
            }
            G[j][i] = G[i][j]; // Граф неориентированный
        }
    }
    return G;
}

int** createGManual(int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
    }

    printf("Введите веса рёбер (0 если ребра нет):\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                G[i][j] = 0; // Вес ребра до самой себя равен 0
            }
            else {
                printf("Вес ребра %d-%d: ", i, j);
                scanf("%d", &G[i][j]);
            }
            G[j][i] = G[i][j]; // Граф неориентированный
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
        if (mstSet[v] == 0 && key[v] < min)//если вершина не была посещена и вес рассматриваемой меньше последнего(уже рассмотренной и установленой в min) 
        {
            min = key[v];//значение в мин обновится
            min_index = v;//будет выбран индекс, соответствующий этой вершине с наименьшим весом
        }
    }
    return min_index;//вернуть индекс вершины с минимальным весом
}

int** primMST(int** graph, int size) {
    int* parent = (int*)malloc(size * sizeof(int)); // список индексом родителей
    int* key = (int*)malloc(size * sizeof(int)); // список минимальных весов
    int* mstSet = (int*)malloc(size * sizeof(int)); // список посещенных (включенных) вершин
    int** mstGraph = (int**)malloc(size * sizeof(int*)); // возвращаемая матрица смежности

    // Инициализируем матрицу смежности для MST
    for (int i = 0; i < size; i++) {
        mstGraph[i] = (int*)calloc(size, sizeof(int)); // Инициализируем нулями
    }

    for (int i = 0; i < size; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    key[0] = 0; // выставляется 0 для первого элемента, так как расстояние равно 0
    parent[0] = -1; // выставляется -1, т.к. у первого элемента нет родителей

    int visitedCount = 0; // счетчик посещенных вершин

    while (visitedCount < size) { // продолжаем, пока не посетим все вершины
        int u = minKey(key, mstSet, size); // u - индекс в строке с минимальным весом 
        mstSet[u] = 1; // выставляется 1, так как мы посетили эту вершину
        visitedCount++; // увеличиваем счетчик посещенных вершин

        for (int v = 0; v < size; v++) {
            if (graph[u][v] && mstSet[v] == 0 && graph[u][v] < key[v]) { // если в исходном графе по минимальному индексу столбца и очередному индексу строки И эта вершина не рассматривалась ранее (MSTset) И значение в этой рассматриваемой ячейке меньше минимального пути
                parent[v] = u; // значение очередного родителя будет перезаписано, индексом минимального ребра
                key[v] = graph[u][v]; // по очередному индексу ребра будет записан вес ребра
            }
        }
    }

    // Заполнение матрицы смежности для MST
    for (int i = 1; i < size; i++) {
        mstGraph[parent[i]][i] = graph[parent[i]][i];
        mstGraph[i][parent[i]] = graph[parent[i]][i];
    }

    free(parent);
    free(key);
    free(mstSet);

    return mstGraph; // Возвращаем матрицу смежности для MST
}

void saveToFile(int** graph, int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи.\n");
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
    printf("Результаты сохранены в файл %s.\n", filename);
}

int main() {
    srand(time(NULL)); // Инициализация генератора случайных чисел
    setlocale(LC_ALL, "RU");

    int size = 0;
    char option;
    int** lastGraph = NULL; // Для хранения последнего созданного графа
    int** lastMST = NULL; // Для хранения последнего минимального остовного дерева

    while (1) {
        printf("Меню:\n");
        printf("1. Задать кол-во вершин графа\n");
        printf("2. Создать граф случайно\n");
        printf("3. Создать граф вручную\n");
        printf("4. Вычислить минимальное остовное дерево\n");
        printf("5. Сохранить результаты в файл\n");
        printf("6. Выход\n");
        printf("Выберите опцию: ");
        scanf(" %c", &option);
        {
            switch (option) {
            case '1':
                printf("Введите кол-во вершин графа: ");
                scanf("%d", &size);
                break;

            case '2': {
                if (size <= 0) {
                    printf("Сначала задайте кол-во вершин графа.\n");
                    break;
                }
                lastGraph = createG(size);
                printf("Сгенерированный граф:\n");
                printG(lastGraph, size);
                lastMST = primMST(lastGraph, size);
                break;
            }

            case '3': {
                if (size <= 0) {
                    printf("Сначала задайте кол-во вершин графа.\n");
                    break;
                }
                lastGraph = createGManual(size);
                printf("Введенный граф:\n");
                printG(lastGraph, size);
                lastMST = primMST(lastGraph, size);
                break;
            }

            case '4': {
                if (lastGraph == NULL) {
                    printf("Сначала создайте граф.\n");
                    break;
                }
                lastMST = primMST(lastGraph, size);
                printf("Матрица смежности минимального остовного дерева:\n");
                printG(lastMST, size);
                break;
            }

            case '5': {
                if (lastGraph == NULL) {
                    printf("Сначала создайте граф.\n");
                    break;
                }
                printf("Сохранить в файл (введите имя файла): ");
                char filename[100];
                scanf("%s", filename);
                saveToFile(lastMST, size, filename);
                break;
            }

            case '6':
                // Освобождение памяти перед выходом
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
                printf("Неверный выбор, попробуйте снова.\n");
            }
        }
    }
    return 0;
}
