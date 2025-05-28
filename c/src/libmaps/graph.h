#ifndef Graph_H_
#define Graph_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HashTableTAB_SIZE 100
#define MAXSTR strlen("Stantsionno-Oyashinskiy")

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

Graph* graph_create(int n);
void graph_destroy(Graph* graph);
void add_edge(Graph* graph, unsigned int i, unsigned int j, int len, int speed);
unsigned int ELFHash(char* s);
HashTable* hashtab_create();
void hashtab_destroy(HashTable* table);
unsigned int hashtab_add(HashTable* hashtab, char* key);
int hashtab_lookup(HashTable* hashtab, char* key);
bool is_in_table(HashTable* table, char* key);
void graph_init(Graph* graph, HashTable* table, FILE* fp);
void show_graph(int v, Edge** mass);

#endif
