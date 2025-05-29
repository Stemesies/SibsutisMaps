#include <libmaps/search.h>

void Dfs(int src, int res, Graph* graph, Path* current_path, PathsContain* all)
{
    if (graph->visited[src] == true)
        return;

    graph->visited[src] = true;

    /*Если входная вершина равна результирующей и путь заполнен, вставляем в
     * конец итоговую вершину и удаляем её и предыдущую с конца списка.*/
    if (src == res) {
        insert_in_path(
                current_path,
                res,
                &(graph->graph_matrix[res][current_path->tail->num]));
        insert_in_path_contain(all, current_path);
        graph->visited[src] = false;
        pop_back(current_path);
        pop_back(current_path);
        return;
    }

    for (int i = 0; i < graph->n_verticles; i++) {
        if (graph->graph_matrix[src][i].len > 0 && !(graph->visited[i])) {
            insert_in_path(current_path, i, &(graph->graph_matrix[src][i]));
            Dfs(i, res, graph, current_path, all);
        }
    }
    pop_back(current_path);
    graph->visited[src] = false;
}

PathsContain* SearchAllPaths(int src, int res, Graph* graph)
{
    PathsContain* all_paths = def_path_contain_construct();
    Path* supp = def_path_construct(src);
    Dfs(src, res, graph, supp, all_paths);
    /*Очищаем не весь путь, а только его оболочку, потому что узлы его очищены и
     * указывают вникуда*/
    free(supp);
    return all_paths;
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
