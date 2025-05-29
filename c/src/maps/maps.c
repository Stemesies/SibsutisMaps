#include <maps/maps.h>

Map* fetch_map_data()
{
    Map* map = map_create();

    if (map == NULL)
        return NULL;

    FILE* fp = fopen(DATABASE_NAME, "r");

    if (fp == NULL) {
        map_destroy(map);
        printf("[ОШИБКА] Невозможно открыть файл \"%s\".\n", DATABASE_NAME);
        printf("Возможно его не существует?\n.");
    }

    graph_init(map->graph, map->hashtable, fp);
    fclose(fp);

    return map;
}

void pointlist_to_idarray() {

}

int construct_paths(MapConfig* mapconfig)
{
    Map* map = fetch_map_data();

    if (map == NULL)
        return -1;

    HashTable* table = map->hashtable;

    int point_ids_size = mapconfig->points->size;
    int* point_ids = calloc(point_ids_size, sizeof(int));

    if(point_ids == NULL) {
        printf("[ОШИБКА] Невозможно выделить память под массив идентификаторов.\n");
        printf("Недостаточно ОЗУ для работы программы.\n.");
        map_destroy(map);
        return -1;
    }

    int k = 0;
    list_foreach_inlined(mapconfig->points, {
        if(!is_in_table(table, list_itp(char))) {
            printf("Неизвестная точка \"%s\"\n", list_itp(char));
            map_destroy(map);
            return -1;
        } else
            point_ids[k++] = hashtab_lookup(table, list_itp(char));
    });
    
    PathsContain* path = def_path_contain_construct();

    int src = point_ids[0];
    int res = point_ids[point_ids_size-1];

    Dfs(src, res, path, map->graph);

    //  PathsContain* path = SearchAllPaths(
    //         hashtab_lookup(table, "Kolyvan"),
    //         hashtab_lookup(table, "Krivodanovka"),
    //         graph);
    // PathsContain* sorted_paths = sort_paths(path, QUICKEST);
    // show_paths(sorted_paths, table);
    // // show_paths(path, table);

    // Path* merge_path = path_with_return(path->first, path->first->next);
    // // if (!merge_path)
    // //     puts("oh noo()");

    printf("Обход завершен.\n");

    PathsContain* corrected_paths = correct_paths(path, res);
    PathsContain* trimmed_paths = trim_paths(corrected_paths, mapconfig->limit);
    PathsContain* filtered_paths = filter_paths(trimmed_paths, point_ids+1, point_ids_size-2);

    free(point_ids);
    point_ids = NULL;

    if(filtered_paths->count == 0) {
        printf("Не получилось проложить маршрут.\n");
        map_destroy(map);
        destroy_paths_contain(filtered_paths);
        return -1;
    }

    PathsContain* sorted_paths = sort_paths(filtered_paths, mapconfig->priority);
    destroy_paths_contain(filtered_paths);

    SearchContext context = {map, sorted_paths, mapconfig, src, res};

    // Выводим информацию о лучшем пути
    print_path(best_path(&context), table, 1);

    // Выводим информацию об альтернативных путях
    if (mapconfig->altways_count > 0)
        alternative(&context);

    // Path* merge_path
    //         = path_with_return(sorted_paths->first, sorted_paths->first->next);
    // if (!merge_path)
    //     puts("oh noo()");

    // print_path(merge_path, table, 5);

    // destroy_path(merge_path);

    map_destroy(map);
    destroy_paths_contain(sorted_paths);

    return 0;
}