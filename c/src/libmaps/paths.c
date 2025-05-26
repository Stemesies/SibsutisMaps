#include <libmaps/paths.h>

QUEUE *queue_create() {
  QUEUE *queue = malloc(sizeof(QUEUE));
  queue->size = 0;
  queue->head = NULL;
  queue->tail = NULL;
  return queue;
}

void queue_add(QUEUE *queue, int num, EDGE *edge) {
  if (!queue) {
    exit(EXIT_FAILURE);
  }
  NODE *insert = def_node_construct(num);
  insert->edge = edge;
  if (queue->size == 0) {
    queue->head = insert;
    queue->tail = insert;
  } else {
    queue->tail->next = insert;
    queue->tail = insert;
  }
  queue->size++;
}

NODE *queue_take(QUEUE *queue) {
  if (queue->size > 0) {
    NODE *curr = queue->head;
    // if (queue->head->next != NULL)
    queue->head = queue->head->next;
    queue->size--;
    return curr;
  }
  return NULL;
}

NODE *def_node_construct(int src) {
  NODE *node_list = malloc(sizeof(NODE));
  node_list->edge = calloc(1, sizeof(EDGE));
  node_list->num = src;
  node_list->next = NULL;
  return node_list;
}

void destroy_node(NODE *node) {
  free(node->edge);
  free(node);
}

LIST *def_list_construct(int src) {
  LIST *list = malloc(sizeof(LIST));
  list->head = def_node_construct(src);
  // list->visited = calloc(10, sizeof(bool)); // 10 переделать
  // list->visited[src] = true;
  list->tail = NULL;
  list->next = NULL;
  list->path = 0;
  list->time = 0;
  return list;
}

PATHS *def_path_construct() {
  PATHS *paths = malloc(sizeof(PATHS));
  paths->count = 0;
  paths->first = NULL;
  paths->last = NULL;
  return paths;
}

void insert_in_list(LIST *list, int num, EDGE *edge) {
  if (!list) {
    exit(EXIT_FAILURE);
  }
  NODE *insert = def_node_construct(num);
  insert->edge = edge;
  if (list->head->next == NULL) {
    list->head->next = insert;
    list->tail = insert;
  } else {
    list->tail->next = insert;
    list->tail = insert;
  }
  list->path += edge->len;
  list->time += ((double)edge->len / (double)edge->speed);
  // list->visited[num] = true;
}

void insert_in_path(PATHS *path, LIST *insert) {
  if (!path || !insert) {
    exit(EXIT_FAILURE);
  }
  if (path->count == 0) {
    path->first = insert;
    path->last = insert;
  } else {
    path->last->next = insert;
    path->last = insert;
  }
  path->count++;
}

LIST *copy_list(LIST *src, int num) // num - до какой вершины копировать список
{
  if (!src)
    return NULL;
  LIST *res = def_list_construct(src->head->num);
  if (src->head->num == num)
    return res;
  NODE *current = src->head->next;
  while (current != NULL) {
    // NODE *insert = def_node_construct();
    insert_in_list(res, current->num, current->edge);
    if (current->num == num)
      break;
    current = current->next;
  }
  return res;
}

bool is_visited(LIST *src, int num) {
  if (!src)
    return false;
  for (NODE *curr = src->head; curr != NULL; curr = curr->next) {
    if (curr->num == num)
      return true;
  }
  return false;
}

void func(LIST *path, bool *visited) {
  if (path && path->head && path->head->next) {
    for (NODE *curr = path->head->next->next; curr != NULL; curr = curr->next) {
      visited[curr->num] = false;
    }
  }
}

/*Демонстрация графа как матрицы смежности*/
void show_graph(int v, EDGE **mass) {
  for (int i = 0; i < v; i++) {
    for (int j = 0; j < v; j++) {
      printf("%d\t", mass[i][j].len);
    }
    putchar('\n');
  }
}

void show_paths(PATHS *paths) {
  int count = 0;
  for (LIST *curr = paths->first; curr != NULL; curr = curr->next) {
    count++;
    printf("Путь %d: ", count);

    for (NODE *temp = curr->head; temp != NULL; temp = temp->next)
      printf("%d->", temp->num);
    printf(": %d\n", curr->path);
  }
}
