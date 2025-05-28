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

    Graph* graph = map->graph;
    HashTable* table = map->hashtable;
    PathsContain* path = def_path_contain_construct();

    int departure_id
            = hashtab_lookup(table, list_firstof(char, mapconfig->points));
    int destination_id
            = hashtab_lookup(table, list_lastof(char, mapconfig->points));

    Dfs(departure_id, destination_id, path, graph);
    PathsContain* new_paths = correct_paths(path, destination_id);

    // FIXME Добавить проверку на необходимость поиска n альтернативных путей
    PathsContain* sorted_paths = NULL;
    if (!sorted_paths)
        sorted_paths = sort_paths(new_paths, QUICKEST);
    print_path(sorted_paths->first, table, 4);
    
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

    map_destroy(map);

    destroy_paths_contain(new_paths);
    destroy_paths_contain(sorted_paths);

    return 0;
}