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

int construct_paths(MapConfig* mapconfig)
{
    Map* map = fetch_map_data();

    if (map == NULL)
        return -1;

    HashTable* table = map->hashtable;
    PathsContain* path = def_path_contain_construct();

    int src = hashtab_lookup(table, list_firstof(char, mapconfig->points));
    int res = hashtab_lookup(table, list_lastof(char, mapconfig->points));

    SearchContext context = {map, path, mapconfig, src, res};

    Dfs(&context);

    PathsContain* new_paths = correct_paths(path, res);
    PathsContain* sorted_paths = sort_paths(new_paths, mapconfig->priority);

    context.paths = sorted_paths;
    destroy_paths_contain(new_paths);

    // Выводим информацию о лучшем пути
    print_path(best_path(&context), table, 1);

    // Выводим информацию об альтернативных путях
    if (mapconfig->altways_count > 0)
        alternative(&context);

    map_destroy(map);

    destroy_paths_contain(sorted_paths);

    return 0;
}