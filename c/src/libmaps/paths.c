/*Файл для работы со структурами хранения путей. Очередь может использоваться
 * при реализации Bfs, если решим использовать его в качестве алгоритма поиска
 * путей. Связный список используется как хранение одного пути, структура path
 * представляет собой список путей.*/
#include <libmaps/paths.h>

NODE* def_node_construct(int src)
{
    NODE* node_list = malloc(sizeof(NODE));
    node_list->edge = calloc(1, sizeof(EDGE));
    node_list->num = src;
    node_list->next = NULL;
    return node_list;
}

void destroy_node(NODE* node)
{
    free(node->edge);
    free(node);
}

QUEUE* queue_create()
{
    QUEUE* queue = malloc(sizeof(QUEUE));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void destroy_queue(QUEUE* a)
{
    NODE* temp = NULL;
    if (!a)
        return;
    for (NODE* curr = a->head; curr != NULL;) {
        temp = curr;
        curr = curr->next;
        destroy_node(temp);
    }
    free(a);
}

void queue_add(QUEUE* queue, int num, EDGE* edge)
{
    if (!queue) {
        exit(EXIT_FAILURE);
    }
    NODE* insert = def_node_construct(num);
    insert->edge->len = edge->len;
    insert->edge->speed = edge->speed;

    if (queue->size == 0) {
        queue->head = insert;
        queue->tail = insert;
    } else {
        queue->tail->next = insert;
        queue->tail = insert;
    }
    queue->size++;
}

NODE* queue_take(QUEUE* queue)
{
    if (queue->size > 0) {
        NODE* curr = queue->head;
        queue->head = queue->head->next;
        queue->size--;
        return curr;
    }
    return NULL;
}

LIST* def_list_construct(int src)
{
    LIST* list = malloc(sizeof(LIST));
    list->head = def_node_construct(src);
    list->tail = NULL;
    list->next = NULL;
    list->path = 0;
    list->time = 0;
    return list;
}

void destroy_list(LIST* a)
{
    NODE* temp = NULL;
    if (!a)
        return;
    for (NODE* curr = a->head; curr != NULL;) {
        temp = curr;
        curr = curr->next;
        destroy_node(temp);
    }
    free(a);
}

PATHS* def_path_construct()
{
    PATHS* paths = malloc(sizeof(PATHS));
    paths->count = 0;
    paths->first = NULL;
    paths->last = NULL;
    return paths;
}

void destroy_paths(PATHS* paths)
{
    LIST* temp = NULL;
    if (!paths)
        return;
    for (LIST* curr = paths->first; curr != NULL;) {
        temp = curr;
        curr = curr->next;
        destroy_list(temp);
    }
    free(paths);
}

/*Головной элемент списка всегда занят исходной вершиной, поэтому начинаем
 * вставку с head->next.*/
void insert_in_list(LIST* list, int num, EDGE* edge)
{
    if (!list) {
        exit(EXIT_FAILURE);
    }
    NODE* insert = def_node_construct(num);
    insert->edge->len = edge->len;
    insert->edge->speed = edge->speed;

    if (list->head->next == NULL) {
        list->head->next = insert;
        list->tail = insert;
    } else {
        list->tail->next = insert;
        list->tail = insert;
    }
    list->path += edge->len;
    list->time += ((double)edge->len / (double)edge->speed);
}

void insert_in_path(PATHS* path, LIST* insert)
{
    if (!path || !insert) {
        exit(EXIT_FAILURE);
    }
    LIST* copy = NULL;
    if (!(insert->tail))
        copy = copy_list(insert, insert->head->num);
    else
        copy = copy_list(insert, insert->tail->num);

    if (path->count == 0) {
        path->first = copy;
        path->last = copy;
    } else {
        path->last->next = copy;
        path->last = copy;
    }
    path->count++;
}

LIST* copy_list(LIST* src, int num) // num - до какой вершины копировать список
{
    if (!src)
        return NULL;
    LIST* res = def_list_construct(src->head->num);
    if (src->head->num == num)
        return res;
    NODE* current = src->head->next;
    while (current != NULL) {
        insert_in_list(res, current->num, current->edge);
        if (current->num == num)
            break;
        current = current->next;
    }
    return res;
}

bool is_visited(LIST* src, int num)
{
    if (!src)
        return false;
    for (NODE* curr = src->head; curr != NULL; curr = curr->next) {
        if (curr->num == num)
            return true;
    }
    return false;
}

void print_path(LIST* path, HASH* table, int count)
{
    printf("Путь %d: ", count);
    for (NODE* temp = path->head; temp != NULL; temp = temp->next)
        printf("%s->", table[temp->num].key);
    printf(": %d км, %.2lf ч\n", path->path, path->time);
}

void show_paths(PATHS* paths, HASH* table, int res)
{
    int count = 0;
    for (LIST* curr = paths->first; curr != NULL; curr = curr->next) {
        if (curr->tail->num == res) {
            count++;
            print_path(curr, table, count);
        }
    }
}

/*Возвращает количество вершин из a, которых нет в b. Может быть полезно при
 * реализации пути с возвратом в исходную точку, чтобы не идти по одинаковому
 * пути туда и обратно. Трудоёмкий метод, O(n*k), где n и k - кол-во вершин в
 * списках. Не рекоммендуется к использованию.*/
int compare_paths(LIST* a, LIST* b)
{
    int count = 0;
    if (!a || !b)
        return -1;
    for (NODE* curr = a->head; curr; curr = curr->next) {
        if (!is_visited(b, curr->num))
            count++;
    }
    return count;
}
