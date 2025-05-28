#ifndef SRCH_H_
#define SRCH_H_

#include <libmaps/graph.h>
#include <libmaps/paths.h>
#include <libmaps/mapconfig.h>

/*Модифицированных обход в глубину. Находит и записывает в контейнер путей все,
 * даже которые не дошли до нужной вершины.*/
void Dfs(int src, int res, PathsContain* path, Graph* graph);
/*Обход в ширину. Резерв обхода в глубину, скорее всего будет выпилен.*/
void Bfs(int src, int res, PathsContain* path, size_t n, Edge** graph);
/*Ищет лучший путь по выбранному приоритету*/
Path* best_path(PathsContain* path, Priority what_path, int res);
/*Ищет альтернативные пути, в rate раз медленнее/дольше/короче лучшего*/
void alternative(
        PathsContain* paths,
        HashTable* table,
        int src,
        int res,
        double rate,
        Priority what_path);

#endif
