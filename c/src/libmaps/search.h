#ifndef SRCH_H_
#define SRCH_H_

#include <libmaps/graph.h>
#include <libmaps/paths.h>
#include <mapconfig/mapconfig.h>

void Dfs(int src, int res, PathsContain* path, Graph* graph);
void Bfs(int src, int res, PathsContain* path, size_t n, Edge** graph);
Path* best_path(PathsContain* path, Priority what_path, int res);
void alternative(
        PathsContain* paths,
        HashTable* table,
        int src,
        int res,
        double rate,
        Priority what_path);

#endif
