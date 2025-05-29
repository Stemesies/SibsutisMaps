#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHTABSIZE 100

// умножаем на 2 т.к. каждый символ кириллицы весит 2 байта.
#define MAXSTR (strlen("Станционно-Ояшинский") )

#define graph_foreach_verticles(i, graph) \
    for (int i = 0; i < (graph)->n_verticles; i++)
#define graph_get(graph, i, j) (graph)->graph_matrix[i][j]

/*Ребро графа*/
typedef struct edge {
    int speed, len;
} Edge;

/*Структура для хранения графа*/
typedef struct graph {
    int n_verticles;
    Edge** graph_matrix;
    bool* visited;
} Graph;

/*Структура для хранения соответствий номеров вершин и населённых пунктов*/
typedef struct hashtable {
    char* key;
    int count;
} HashTable;

/*Объединение графа и таблицы*/
typedef struct map {
    Graph* graph;
    HashTable* hashtable;
} Map;

/* Создает карту, содержащую пустой граф из n вершин и хэш-таблицу.*/
Map* map_create();
void map_destroy(Map* map);

/*Создаёт пустой граф из n вершин.*/
Graph* graph_create(int n);
void graph_destroy(Graph* graph);

/*Добавляет в граф graph ребро {ij} с приоритетом {speed, len}*/
void graph_add_edge(
        Graph* graph, unsigned int i, unsigned int j, int len, int speed);

/*Заполняет параллельно граф и таблицу вершин из входного файла.*/
void graph_init(Graph* graph, HashTable* table, FILE* fp);

/*Демонстрация заполненной матрицы графа*/
void show_graph(int v, Edge** mass);

HashTable* hashtab_create();
void hashtab_destroy(HashTable* table);

/*Хэш-функция для вычисления индекса вершины в таблице. Бывают коллизии! Не
 * используйте хэш-функцию для поиска, для этого есть lookup!*/
unsigned int elf_hash(char* s);

/*Добавление вершины в таблицу по ключу. Для разрешения коллизий используется
 * метод открытой адресации.*/
unsigned int hashtab_add(HashTable* hashtab, char* key);

/*Поиск вершины в таблице по ключу - возвращает индекс, по которому вершина
 * точно лежит в таблице. Если элемент отсутствует, возвращает -1.*/
int hashtab_lookup(HashTable* hashtab, char* key);

char* hashtab_getkey(HashTable* table, int id);

/*Добавлен ли элемент с ключом key в таблицу table?*/
bool hashtab_contains_key(HashTable* table, char* key);

void hashtab_print(HashTable* table);

#endif
