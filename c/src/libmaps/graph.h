#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASHTAB_SIZE 5
#define MAXSTR strlen("Ust-Blagoveschensk")

/*Ребро графа*/
typedef struct edge
{
    int speed, len;
} EDGE;

/*Структура для хранения графа*/
typedef struct graph
{
    int n_verticles;
    EDGE **graph_matrix;
    int *verticles;
    bool *visited;
} GRAPH;

/*Структура для хранения соответствий номеров вершин и населённых пунктов*/
typedef struct listnode
{
    char *key;
} HASH;

GRAPH *graph_create(int n);
void graph_destroy(GRAPH *graph);
void add_edge(GRAPH *graph, unsigned int i, unsigned int j, int len, int speed);
unsigned int ELFHash(char *s);
HASH *hashtab_create();
void hashtab_destroy(HASH *table);
unsigned int hashtab_add(HASH *hashtab, char *key);
int hashtab_lookup(HASH *hashtab, char *key);
bool is_in_table(HASH *table, char *key);
void graph_init(GRAPH *graph, HASH *table, FILE *fp);
void show_graph(int v, EDGE **mass);

#endif
