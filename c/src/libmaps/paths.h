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
    Edge* edge;
    int num;
    struct node* next;
} PathNode;

/*Список вершин (путь)*/
typedef struct list {
    PathNode *head, *tail;
    int path;
    double time;
    // bool *visited;
    struct list* next;
} Path;

/*Структура для хранения путей*/
typedef struct paths {
    Path *first, *last;
    int count;
} PathsContain;

/*Очередь вершин*/
typedef struct queue {
    int size;
    PathNode *head, *tail;
} Queue;

/*Прототипы функций*/

PathNode* def_node_construct(int num);
void destroy_node(PathNode* node);
Queue* queue_create();
void destroy_queue(Queue* a);
void queue_add(Queue* queue, int num, Edge* edge);
PathNode* queue_take(Queue* queue);
Path* def_list_construct(int num);
void destroy_list(Path* a);
PathsContain* def_path_construct();
void destroy_paths(PathsContain* paths);
void insert_in_list(Path* list, int num, Edge* edge);
void insert_in_path(PathsContain* path, Path* insert);
Path* copy_list(Path* src, int num);
bool is_visited(Path* src, int num);
void print_path(Path* path, HashTable* table, int count);
void show_paths(PathsContain* paths, HashTable* is_in_table, int res);
int compare_paths(Path* a, Path* b);
PathsContain* correct_paths(PathsContain* paths, int res);

#endif
