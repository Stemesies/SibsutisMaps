#include <libmaps/search.h>
#include <libmaps/sort.h>

int main()
{
    PathsContain* path = def_path_contain_construct();
    Graph* graph = graph_create(HASHTABSIZE);
    HashTable* table = hashtab_create();
    FILE* fp = fopen("input", "r");

    // graph->graph_matrix[0][1].len = 15;
    // graph->graph_matrix[0][2].len = 10;
    // graph->graph_matrix[0][3].len = 25;
    // graph->graph_matrix[1][4].len = 26;
    // graph->graph_matrix[2][4].len = 8;
    // graph->graph_matrix[3][5].len = 17;
    // graph->graph_matrix[4][5].len = 14;
    // graph->graph_matrix[4][6].len = 19;
    // graph->graph_matrix[5][6].len = 33;

    // graph->graph_matrix[0][1].speed = 80;
    // graph->graph_matrix[0][2].speed = 60;
    // graph->graph_matrix[0][3].speed = 85;
    // graph->graph_matrix[1][4].speed = 60;
    // graph->graph_matrix[2][4].speed = 58;
    // graph->graph_matrix[3][5].speed = 37;
    // graph->graph_matrix[4][5].speed = 69;
    // graph->graph_matrix[4][6].speed = 73;
    // graph->graph_matrix[5][6].speed = 90;

    // for (int i = 0; i < 7; i++) {
    //     for (int j = 0; j < 7; j++)
    //         if (graph->graph_matrix[i][j].len > 0) {
    //             graph->graph_matrix[j][i].len =
    //             graph->graph_matrix[i][j].len;
    //             graph->graph_matrix[j][i].speed
    //                     = graph->graph_matrix[i][j].speed;
    //         }
    // }
    graph_init(graph, table, fp);
    // show_graph(7, graph->graph_matrix);

    Path* curr = def_path_construct(hashtab_lookup(table, "Novosibirsk"));
    Dfs(hashtab_lookup(table, "Novosibirsk"),
        hashtab_lookup(table, "Karasuk"),
        graph,
        curr,
        path);

    // int count = 0;
    // for (Path* curr = path->first; curr != NULL; curr = curr->next) {
    //     count++;
    //     printf("Путь %d: ", count);
    //     PathNode* temp = curr->head;
    //     if (!temp) {
    //         return 1;
    //     }
    //     printf("%d", temp->num);
    //     temp = temp->next;
    //     for (; temp != NULL; temp = temp->next)
    //         printf("->%d", temp->num);
    //     printf(": %d км, %.2lf ч\n", curr->path, curr->time);
    // }

    // FIXME Добавить проверку на необходимость поиска n альтернативных путей
    // PathsContain* new_paths
    //         = correct_paths(path, hashtab_lookup(table, "Novosibirsk"));
    PathsContain* sorted_paths = sort_paths(path, QUICKEST);
    show_paths(sorted_paths, table);
    // show_paths(path, table);

    Path* merge_path = path_with_return(path->first, path->first->next);
    // if (!merge_path)
    //     puts("oh noo()");

    print_path(merge_path, table, 5);

    // alternative(
    //         new_paths,
    //         table,
    //         hashtab_lookup(table, "Novosibirsk"),
    //         hashtab_lookup(table, "Karasuk"),
    //         1.5,
    //         SHORTEST);
    // printf("Karasuk: %d\n", hashtab_lookup(table, "Moshkovo"));
    // printf("%s\n", table[43].key);

    // Path* a = best_path(path, LONGEST, hashtab_lookup(table, "Karasuk"));
    // // printf("%p\n", a);

    // printf("Самый длинный путь из Новосибирска в Карасук: \n");
    // for (PathNode* temp = a->head; temp != NULL; temp = temp->next)
    //     printf("%s->", table[temp->num].key);
    // printf(": %d км, %.2lf ч\n", a->path, a->time);
    graph_destroy(graph);
    hashtab_destroy(table);
    // destroy_paths_contain(new_paths);
    // destroy_paths_contain(sorted_paths);
    // destroy_path(merge_path);
    fclose(fp);

    return 0;
}
