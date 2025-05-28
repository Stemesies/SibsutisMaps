#include <libmaps/search.h>

void Dfs(int src, int res, PathsContain* path, Graph* graph)
{
    if (src == res)
        return;

    if (!path) {
        exit(EXIT_FAILURE);
    }
    graph->visited[src] = true;
    for (int i = 0; i < graph->n_verticles; i++) {
        if (graph->graph_matrix[src][i].len > 0) {
            if (!(graph->visited[i])) {
                Path* new_list = NULL;

                if (graph->verticles[src] > 0) {
                    new_list = copy_path(path->last, src);
                    insert_in_path(new_list, i, &(graph->graph_matrix[src][i]));
                    insert_in_path_contain(path, new_list);
                    // destroy_path(new_list);
                } else {
                    if (!is_visited(path->last, i)) {
                        if (path->count == 0) {
                            new_list = def_path_construct(src);
                            insert_in_path(
                                    new_list,
                                    i,
                                    &(graph->graph_matrix[src][i]));
                            insert_in_path_contain(path, new_list);
                            // destroy_path(new_list);
                        } else
                            insert_in_path(
                                    path->last,
                                    i,
                                    &(graph->graph_matrix[src][i]));
                    }
                }

                graph->verticles[src]++;
                Dfs(i, res, path, graph);
                graph->visited[i] = false;
                destroy_path(new_list);
            }
        }
    }
}

void Bfs(int src, int res, PathsContain* path, size_t n, Edge** graph)
{
    bool* visited = calloc(n, sizeof(bool));
    PathNode* vert;
    Queue* queue = queue_create();
    visited[src] = true;
    Edge src_edge = {0, 0};
    queue_add(queue, src, &src_edge);
    int* verticles = calloc(n, sizeof(int));
    while (queue->size > 0) {
        vert = queue_take(queue);
        if (vert->num == res)
            continue;
        for (int i = 0; i < n; i++) {
            if (graph[vert->num][i].len > 0) {
                queue_add(queue, i, &(graph[vert->num][i]));
                Path* new_list = NULL;

                if (verticles[vert->num] > 0) {
                    for (Path* curr = path->first; curr != NULL;
                         curr = curr->next)
                    // if (curr->tail->num == vert->num && !(curr->visited[i]))
                    {
                        new_list = copy_path(curr, vert->num);
                        insert_in_path(new_list, i, &(graph[vert->num][i]));
                        insert_in_path_contain(path, new_list);
                    }
                } else {
                    if (path->count == 0) {
                        new_list = def_path_construct(src);
                        insert_in_path(new_list, i, &(graph[src][i]));
                        insert_in_path_contain(path, new_list);
                    } else // добавить поиск списка по крайней вершине
                           // (проверять на совпадение с vert->num)
                    {
                        for (Path* curr = path->first; curr != NULL;
                             curr = curr->next)
                            // if (curr->tail->num == vert->num &&
                            // !(curr->visited[i]))
                            insert_in_path(curr, i, &(graph[vert->num][i]));
                    }
                }

                visited[i] = true;
                verticles[vert->num]++;
            }
        }
    }
}

/*Лучший путь по выбранному критерию*/
Path* best_path(PathsContain* path, Priority what_path, int res)
{
    Path *res_short = NULL, *res_long = NULL, *res_quick = NULL;
    int temp_short = INT_MAX, temp_long = 0.0;
    double temp_quick = DBL_MAX;
    for (Path* curr = path->first; curr != NULL; curr = curr->next) {
        if (curr->tail->num == res) {
            if (curr->path < temp_short) {
                res_short = curr;
                temp_short = curr->path;
            }
            if (curr->path > temp_long) {
                res_long = curr;
                temp_long = curr->path;
            }
            if (curr->time < temp_quick) {
                res_quick = curr;
                temp_quick = curr->time;
            }
        }
    }

    switch (what_path) {
    case LONGEST:
        return res_long;
    case SHORTEST:
        return res_short;
    case QUICKEST:
        return res_quick;
    default:
        return NULL;
    }
}

void alternative(
        PathsContain* paths,
        HashTable* table,
        int src,
        int res,
        double rate,
        Priority what_path)
{
    int count = 0;
    Path* a = best_path(paths, what_path, res);
    printf("Альтернативные пути %s - %s:\n", table[src].key, table[res].key);
    for (Path* curr = paths->first; curr != NULL; curr = curr->next) {
        if (curr->tail->num == res) {
            switch (what_path) {
            case LONGEST:
                if (((double)a->path / (double)curr->path) <= rate) {
                    count++;
                    print_path(curr, table, count);
                }
                break;
            case SHORTEST:
                if (((double)curr->path / (double)a->path) <= rate) {
                    count++;
                    print_path(curr, table, count);
                }
                break;
            case QUICKEST:
                if ((curr->time / a->time) <= rate) {
                    count++;
                    print_path(curr, table, count);
                }
                break;

            default:
                break;
            }
        }
    }
}
