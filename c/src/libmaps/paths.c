/*Файл для работы со структурами хранения путей. Очередь может использоваться
 * при реализации Bfs, если решим использовать его в качестве алгоритма поиска
 * путей. Связный список используется как хранение одного пути, структура path
 * представляет собой список путей.*/
#include <libmaps/paths.h>

PathNode* def_node_construct(int src)
{
    PathNode* node_list = malloc(sizeof(PathNode));
    node_list->edge = calloc(1, sizeof(Edge));
    node_list->num = src;
    node_list->next = NULL;
    return node_list;
}

void destroy_node(PathNode* node)
{
    free(node->edge);
    free(node);
}

Queue* queue_create()
{
    Queue* queue = malloc(sizeof(Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void destroy_queue(Queue* a)
{
    PathNode* temp = NULL;
    if (!a)
        return;
    for (PathNode* curr = a->head; curr != NULL;) {
        temp = curr;
        curr = curr->next;
        destroy_node(temp);
    }
    free(a);
}

void queue_add(Queue* queue, int num, Edge* edge)
{
    if (!queue) {
        exit(EXIT_FAILURE);
    }
    PathNode* insert = def_node_construct(num);
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

PathNode* queue_take(Queue* queue)
{
    if (queue->size > 0) {
        PathNode* curr = queue->head;
        queue->head = queue->head->next;
        queue->size--;
        return curr;
    }
    return NULL;
}

Path* def_path_construct(int src)
{
    Path* list = malloc(sizeof(Path));
    list->head = def_node_construct(src);
    list->tail = NULL;
    list->next = NULL;
    list->path = 0;
    list->time = 0;
    return list;
}

void destroy_path(Path* a)
{
    PathNode* temp = NULL;
    if (!a)
        return;
    for (PathNode* curr = a->head; curr != NULL;) {
        temp = curr;
        curr = curr->next;
        destroy_node(temp);
    }
    free(a);
}

PathsContain* def_path_contain_construct()
{
    PathsContain* paths = malloc(sizeof(PathsContain));
    paths->count = 0;
    paths->first = NULL;
    paths->last = NULL;
    return paths;
}

void destroy_paths_contain(PathsContain* paths)
{
    Path* temp = NULL;
    if (!paths)
        return;
    for (Path* curr = paths->first; curr != NULL;) {
        temp = curr;
        curr = curr->next;
        destroy_path(temp);
    }
    free(paths);
}

/*Головной элемент пути всегда занят исходной вершиной, поэтому начинаем
 * вставку с head->next.*/
void insert_in_path(Path* list, int num, Edge* edge)
{
    if (!list) {
        exit(EXIT_FAILURE);
    }
    PathNode* insert = def_node_construct(num);
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

void insert_in_path_contain(PathsContain* path, Path* insert)
{
    if (!path || !insert) {
        exit(EXIT_FAILURE);
    }
    Path* copy = NULL;
    if (!(insert->tail))
        copy = copy_path(insert, insert->head->num);
    else
        copy = copy_path(insert, insert->tail->num);

    if (path->count == 0) {
        path->first = copy;
        path->last = copy;
    } else {
        path->last->next = copy;
        path->last = copy;
    }
    path->count++;
}

Path* copy_path(
        const Path* src, int num) // num - до какой вершины копировать список
{
    if (!src)
        return NULL;
    Path* res = def_path_construct(src->head->num);
    if (src->head->num == num)
        return res;
    PathNode* current = src->head->next;
    while (current != NULL) {
        insert_in_path(res, current->num, current->edge);
        if (current->num == num)
            break;
        current = current->next;
    }
    return res;
}

bool is_visited(Path* src, int num)
{
    if (!src)
        return false;
    for (PathNode* curr = src->head; curr != NULL; curr = curr->next) {
        if (curr->num == num)
            return true;
    }
    return false;
}

void print_path(const Path* path, const HashTable* table, int count)
{
    printf("Путь %d: ", count);
    PathNode* temp = path->head;
    if (!temp) {
        return;
    }
    printf("%s", table[temp->num].key);
    temp = temp->next;
    for (; temp != NULL; temp = temp->next)
        printf("->%s", table[temp->num].key);
    printf(": %d км, %.2lf ч\n", path->path, path->time);
}

void show_paths(const PathsContain* paths, const HashTable* table)
{
    int count = 0;
    for (Path* curr = paths->first; curr != NULL; curr = curr->next) {
        print_path(curr, table, ++count);
    }
}

/*Возвращает количество вершин из a, которых нет в b. Может быть полезно при
 * реализации пути с возвратом в исходную точку, чтобы не идти по одинаковому
 * пути туда и обратно. Трудоёмкий метод, O(n*k), где n и k - кол-во вершин в
 * списках. Не рекоммендуется к использованию.*/
int compare_paths(Path* a, Path* b)
{
    int count = 0;
    if (!a || !b)
        return -1;
    for (PathNode* curr = a->head; curr; curr = curr->next) {
        if (!is_visited(b, curr->num))
            count++;
    }
    return count;
}

/*Поиск вершины в пути по её номеру*/
int path_lookup(Path* path, int vert)
{
    int index = 0;
    for (PathNode* curr = path->head; curr != NULL; curr = curr->next) {
        if (curr->num == vert)
            return index;
        index++;
    }
    return -1;
}

/*Последовательно ли идут вершины в пути?*/
bool is_in_order(Path* path, int* points, int points_count)
{
    if (points_count == 2 || points_count == 3)
        return true;
    for (int i = 1; i < points_count; i++) {
        if (path_lookup(path, points[i]) < path_lookup(path, points[i - 1]))
            return false;
    }
    return true;
}

bool path_contains_all(Path* path, int* points, int points_count)
{
    if (points_count == 0)
        return true;

    for (int i = 0; i < points_count; i++) {
        if (!is_visited(path, points[i]))
            return false;
        if (!is_in_order(path, points, points_count))
            return false;
    }

    return true;
}

bool path_fits_limit(Path* path, int limit)
{
    if (limit == 0)
        return true;

    int i = 0;
    for (PathNode* pn = path->head; pn != NULL; pn = pn->next) {
        if (i++ >= limit + 1) {
            return false;
        }
    }
    return true;
}

/*Извлечение узла из начала пути*/
PathNode* pop_node(Path* path)
{
    if (!path || !(path->head))
        return NULL;
    PathNode* res = path->head;
    path->head = path->head->next;
    res->next = NULL;

    return res;
}

Path* path_with_return(const Path* path_to, Path* path_back)
{
    if (!path_to || !path_back)
        return NULL;
    Path* res = copy_path(path_to, path_to->tail->num);
    PathNode *next = NULL,
             *tail = NULL; // tail запоминает элемент, который
                           // был добавлен первым в путь res
    int i = 0; // счётчик итераций цикла (нужен, чтобы отследить добавление
               // первого элемента, на который в итоге будет передвинут
               // res->tail)
    /*В цикле - temp запоминает текущий tail, чтобы потом к нему возвращаться,
     * сдвигая вставленные ранее элементы дальше в конец (нужно для получения
     * реверса); next запоминает, куда будет указывать вновьдобавленный
     * элемент*/
    while (path_back->head != NULL && path_back->head != path_back->tail) {
        PathNode* insert = pop_node(path_back);
        PathNode* temp = res->tail;
        insert_in_path(res, insert->num, insert->edge);
        if (i == 0)
            tail = res->tail;

        res->tail->next = next;
        next = res->tail;
        res->tail = temp;
        destroy_node(insert);
        i++;
    }
    res->path = path_to->path + path_back->path;
    res->time = path_to->time + path_back->time;
    res->tail = tail;

    return res;
}

void pop_back(Path* path)
{
    PathNode* pop = path->tail;
    for (PathNode* curr = path->head; curr != NULL; curr = curr->next) {
        if (curr->next == path->tail) {
            path->tail = curr;
            curr->next = NULL;
            path->path -= pop->edge->len;
            path->time -= (double)((double)pop->edge->len
                                   / (double)pop->edge->speed);
            break;
        }
    }
    destroy_node(pop);
}
