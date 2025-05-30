#ifndef PATH_H_
#define PATH_H_

#include <float.h>
#include <libmaps/graph.h>
#include <limits.h>

/*Узел списка*/
typedef struct node {
    Edge* edge;
    int num;
    struct node* next;
} PathNode;

/*Список вершин (путь)*/
typedef struct path {
    PathNode *head, *tail;
    int path;
    double time;
    struct path* next;
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

/*Создаёт узел пути, инициализируя расстояние и путь нулями (так как вершина -
 * начальная), а номер вершины - переданным значением src.*/
PathNode* def_node_construct(int src);
void destroy_node(PathNode* node);
Queue* queue_create();
void destroy_queue(Queue* a);
void queue_add(Queue* queue, int num, Edge* edge);
PathNode* queue_take(Queue* queue);
/*Создаёт путь, помещая в него изначальную вершину.*/
Path* def_path_construct(int src);
void destroy_path(Path* a);
PathsContain* def_path_contain_construct();
void destroy_paths_contain(PathsContain* paths);

/*Вставляет вершину num и соответствующие ей приоритеты ребра edge в путь
 * list. Головной элемент пути всегда занят исходной вершиной, поэтому начинаем
 * вставку с head->next.*/
void insert_in_path(Path* list, int num, Edge* edge);

/*Вставляет новый путь в контейнер путей. */
void insert_in_path_contain(PathsContain* path, Path* insert);

/*Копирует путь src до вершины num*/
Path* copy_path(const Path* src, int num);

/*Есть ли уже вершина num в пути path?*/
bool is_visited(Path* path, int num);

void print_path(const Path* path, const HashTable* table, int count);
void show_paths(const PathsContain* paths, const HashTable* is_in_table);

/*Возвращает количество вершин из a, которых нет в b. Может быть полезно при
реализации пути с возвратом в исходную точку, чтобы не идти по одинаковому
пути туда и обратно. Трудоёмкий метод, O(n*k), где n и k - кол-во вершин в
списках. Не рекоммендуется к использованию.*/
int compare_paths(Path* a, Path* b);

/* Проверяет все ли населенные пункты points посещены в данном маршруте. */
bool path_contains_all(Path* path, int* points, int points_count);

/* Проверяет данный маршрут на превышение количества населенных пунктов */
bool path_fits_limit(Path* path, int limit);

/*Извлекает узел из начала пути. Устанавливает указатель этого узла на следующий
 * в NULL.*/
PathNode* pop_node(Path* path);

/*Объединяет путь path_to с реверсированным путём path_back (без результирующей
 * вершины из path_back - предполагается, что концы путей совпадают, и мы уже
 * находимся в этой вершине.)*/
Path* path_with_return(const Path* path_to, Path* path_back);

/*Удаление вершины с конца пути*/
void pop_back(Path* path);

/*Поиск вершины в пути по её номеру*/
int path_lookup(Path* path, int vert);

/*По порядку ли расположены вершины в пути?*/
bool is_in_order(Path* path, int* points, int points_count);

#endif
