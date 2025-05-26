/*Предварительный файл для определения структуры ребра, необходимой для
реализации хранения путей. Потом будет слит с основным файлом из ветки issue-6*/
#ifndef GRAPH_H_
#define GRAPH_H

#include <stdbool.h>
#define HASHTAB_SIZE 50

typedef struct edge
{
    double speed;
    int len;
} EDGE;

typedef struct graph
{
    int n_verticles;
    EDGE **graph_matrix;
    int *verticles;
    bool *visited;
} GRAPH;

#endif
