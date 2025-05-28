#ifndef Graph_H_
#define Graph_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HashTableTAB_SIZE 100
#define MAXSTR (strlen("Станционно-Ояшинский") * 2)

/*Ребро графа*/
typedef struct edge {
    int speed, len;
} Edge;

/*Структура для хранения графа*/
typedef struct graph {
    int n_verticles;
    Edge** graph_matrix;
    int* verticles; // массив verticles отвечает за количество посещений каждой
                    // вершины
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

/*Прототипы функций*/

/* Создает карту, содержащую пустой граф из n вершин и хэш-таблицу.*/
Map* map_create();
void map_destroy(Map* map);

/*Создаёт пустой граф из n вершин.*/
Graph* graph_create(int n);
void graph_destroy(Graph* graph);

/*Добавляет в граф graph ребро {ij} с приоритетом {speed, len}*/
void add_edge(Graph* graph, unsigned int i, unsigned int j, int len, int speed);

/*Хэш-функция для вычисления индекса вершины в таблице. Бывают коллизии! Не
 * используйте хэш-функцию для поиска, для этого есть lookup!*/
unsigned int ELFHash(char* s);
HashTable* hashtab_create();
void hashtab_destroy(HashTable* table);

/*Добавление вершины в таблицу по ключу. Для разрешения коллизий используется
 * метод открытой адресации.*/
unsigned int hashtab_add(HashTable* hashtab, char* key);

/*Поиск вершины в таблице по ключу - возвращает индекс, по которому вершина
 * точно лежит в таблице. */
int hashtab_lookup(HashTable* hashtab, char* key);

/*Добавлен ли элемент с ключом key в таблицу table?*/
bool is_in_table(HashTable* table, char* key);

/*Заполняет параллельно граф и таблицу вершин из входного файла.*/
void graph_init(Graph* graph, HashTable* table, FILE* fp);

/*Демонстрация заполненной матрицы графа*/
void show_graph(int v, Edge** mass);

#endif
