#ifndef SRCH_H_
#define SRCH_H_

#include <libmaps/graph.h>
#include <libmaps/paths.h>
#define SHORTEST 0
#define LONGEST 1
#define QUICKEST 2

void Dfs(int src, int res, PATHS* path, GRAPH* graph);
void Bfs(int src, int res, PATHS* path, size_t n, EDGE** graph);
LIST* best_path(PATHS* path, int what_path, int res);

#endif
