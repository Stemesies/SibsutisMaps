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
        return NULL;
    }

    graph_init(map->graph, map->hashtable, fp);
    fclose(fp);

    return map;
}

int pointlist_to_idarray(MapConfig* mapconfig, int* point_ids, HashTable* table)
{
    int k = 0;
    list_foreach_inlined(mapconfig->points, {
        if (!hashtab_contains_key(table, list_itp(char))) {
            printf("Неизвестная точка \"%s\"\n", list_itp(char));
            return -1;
        } else
            point_ids[k++] = hashtab_lookup(table, list_itp(char));
    });
    return 0;
}

int construct_paths(MapConfig* mapconfig)
{
    Map* map = fetch_map_data();
    if (map == NULL)
        return -1;

    SearchContext context
            = {.map = map,
               .paths = NULL,
               .config = mapconfig,
               .src = -1,
               .res = -1,
               .input_points_size = mapconfig->points->size,
               .input_points = calloc(mapconfig->points->size, sizeof(int))};

    if (context.input_points == NULL) {
        printf("[ОШИБКА] Невозможно выделить память под массив "
               "идентификаторов.\n");
        printf("Недостаточно ОЗУ для работы программы.\n.");
        map_destroy(map);
        return -1;
    }

    // Конвертируем названия населенных пунктов в айдиншики + проверяем
    // на правильность ввода.
    if (pointlist_to_idarray(
                mapconfig, context.input_points, context.map->hashtable)
        == -1) {
        map_destroy(map);
        return -1;
    }

    for (int i = 0; i < context.input_points_size; i++)
    {
        printf("Point %d: %d (%s)\n", i, context.input_points[i], hashtab_getkey(context.map->hashtable, context.input_points[i]));
    }
    

    context.paths = def_path_contain_construct();
    context.src = context.input_points[0];
    context.res = context.input_points[context.input_points_size - 1];

    PathsContain* search_result = search_all_paths(&context);
    printf("Обход завершен.\n");

    free(context.input_points);
    context.input_points = NULL;

    if (search_result == NULL) {
        printf("Не получилось проложить маршрут.\n");
        map_destroy(map);
        destroy_paths_contain(context.paths);
        return -1;
    }

    // Выводим информацию о лучшем пути
    print_path(best_path(context.paths), context.map->hashtable, 1);

    // Выводим информацию об альтернативных путях
    if (mapconfig->altways_count > 0)
        alternative(&context);

    // Path* merge_path = path_with_return(path->first, path->first->next);
    // if (!merge_path)
    //     puts("oh noo()");

    map_destroy(map);
    destroy_paths_contain(context.paths);

    return 0;
}