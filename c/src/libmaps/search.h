#ifndef SRCH_H_
#define SRCH_H_

#include <libmaps/graph.h>
#include <libmaps/paths.h>
#include <mapconfig/mapconfig.h>

void Dfs(int src, int res, PATHS* path, GRAPH* graph);
void Bfs(int src, int res, PATHS* path, size_t n, EDGE** graph);
LIST* best_path(PATHS* path, Priority what_path, int res);
void alternative(
        PATHS* paths,
        HASH* table,
        int src,
        int res,
        double rate,
        Priority what_path);

#endif
