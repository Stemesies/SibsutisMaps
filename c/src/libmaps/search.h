#ifndef SRCH_H_
#define SRCH_H_

#include <libmaps/graph.h>
#include <libmaps/mapconfig.h>
#include <libmaps/paths.h>

typedef struct search_context {
    Map* map;
    PathsContain* paths;
    MapConfig* config;
    int src;
    int res;
} SearchContext;

/*Модифицированных обход в глубину. Находит и записывает в контейнер путей все,
 * даже которые не дошли до нужной вершины.*/
void Dfs(SearchContext* context);
/*Обход в ширину. Резерв обхода в глубину, скорее всего будет выпилен.*/
void Bfs(int src, int res, PathsContain* path, size_t n, Edge** graph);
/*Ищет лучший путь по выбранному приоритету*/
Path* best_path(SearchContext* context);
/*Ищет альтернативные пути, в rate раз медленнее/дольше/короче лучшего*/
void alternative(SearchContext* context);

#endif
