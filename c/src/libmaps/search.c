#include <libmaps/search.h>

#define c_src (context->src)
#define c_res (context->res)
#define c_paths (context->paths)
#define c_graph (context->map->graph)
#define c_table (context->map->hashtable)

bool validate_path(Path* path, SearchContext* context)
{
    if (!path_fits_limit(path, context->config->limit))
        return false;
    print_path(path, c_table, 1);
    if (!path_contains_all(
                path,
                context->input_points,
                context->input_points_size,
                c_table))
        return false;

    return true;
}

void Dfs(SearchContext* context, int src, Path* current_path)
{
    if (c_graph->visited[src] == true)
        return;
    c_graph->visited[src] = true;

    /*Если входная вершина равна результирующей и путь заполнен, вставляем в
     * конец итоговую вершину и удаляем её и предыдущую с конца списка.*/
    if (src == c_res) {
        insert_in_path(
                current_path,
                c_res,
                &(c_graph->graph_matrix[c_res][current_path->tail->num]));

        if (!validate_path(current_path, context)) {
            c_graph->visited[src] = false;
            pop_back(current_path);
        } else {
            insert_in_path_contain(c_paths, current_path);
            printf("(%p) ", c_paths->last);
            print_path(c_paths->last, c_table, 1);
            c_graph->visited[src] = false;
            pop_back(current_path);
            pop_back(current_path);
        }
    }

    for (int i = 0; i < c_graph->n_verticles; i++) {
        if (c_graph->graph_matrix[src][i].len > 0 && !(c_graph->visited[i])) {
            insert_in_path(current_path, i, &(c_graph->graph_matrix[src][i]));
            Dfs(context, i, current_path);
        }
    }
    pop_back(current_path);
    c_graph->visited[src] = false;
}

PathsContain* search_all_paths(SearchContext* context)
{
    Path* supp = def_path_construct(c_src);
    Dfs(context, c_src, supp);
    /*Очищаем не весь путь, а только его оболочку, потому что узлы его очищены и
     * указывают вникуда*/
    free(supp);

    if (context->paths->count == 0) {
        destroy_paths_contain(context->paths);
        context->paths = NULL;
        return NULL;
    }
    // Сортируем найденные пути
    PathsContain* sorted_paths
            = sort_paths(context->paths, context->config->priority);
    destroy_paths_contain(context->paths);
    context->paths = sorted_paths;

    return context->paths;
}

/*Лучший путь по выбранному критерию*/
Path* best_path(PathsContain* paths)
{
    return paths == NULL ? NULL : paths->first;
}

void alternative(SearchContext* context)
{
    int rate = context->config->altways_filter_coefficient;
    int alt_limit = context->config->altways_count;

    int count = 0;
    Path* a = best_path(context->paths);

    printf("Альтернативные пути %s - %s:\n",
           c_table[c_src].key,
           c_table[c_res].key);

    for (Path* curr = c_paths->first; curr != NULL; curr = curr->next) {
        if (curr->tail->num == c_res) {
            // Лучший путь не учитываем, как альтернативный
            if (a == curr)
                continue;

            switch (context->config->priority) {
            case LONGEST:
                if (((double)a->path / (double)curr->path) <= rate) {
                    count++;
                    printf("(%p) ", curr);
                    print_path(curr, c_table, count);
                }
                break;
            case SHORTEST:
                if (((double)curr->path / (double)a->path) <= rate) {
                    count++;
                    printf("(%p) ", curr);
                    print_path(curr, c_table, count);
                }
                break;
            case QUICKEST:
                if ((curr->time / a->time) <= rate) {
                    count++;
                    printf("(%p) ", curr);
                    print_path(curr, c_table, count);
                }
                break;

            default:
                break;
            }
        }

        // Нам не нужно больше, чем alt_limit альтернативных путей
        if (count >= alt_limit)
            break;
    }

    if (count == 0) {
        printf("Альтернативных путей не найдено.\n");
    }
}
