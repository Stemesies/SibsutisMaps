#ifndef PATH_H_
#define PATH_H_

#include <float.h>
#include <libmaps/graph.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*Узел списка*/
typedef struct node {
    EDGE* edge;
    int num;
    struct node* next;
} NODE;

/*Список вершин (путь)*/
typedef struct list {
    NODE *head, *tail;
    int path;
    double time;
    // bool *visited;
    struct list* next;
} LIST;

/*Структура для хранения путей*/
typedef struct paths {
    LIST *first, *last;
    int count;
} PATHS;

/*Очередь вершин*/
typedef struct queue {
    int size;
    NODE *head, *tail;
} QUEUE;

/*Прототипы функций*/
QUEUE* queue_create();
void queue_add(QUEUE* queue, int num, EDGE* edge);
NODE* queue_take(QUEUE* queue);
NODE* def_node_construct(int num);
void destroy_node(NODE* node);
LIST* def_list_construct(int num);
PATHS* def_path_construct();
void insert_in_list(LIST* list, int num, EDGE* edge);
void insert_in_path(PATHS* path, LIST* insert);
LIST* copy_list(LIST* src, int num);
bool is_visited(LIST* src, int num);
void show_paths(PATHS* paths);

#endif
