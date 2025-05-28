#ifndef SORT_H_
#define SORT_H

#include <libmaps/paths.h>
#include <mapconfig/mapconfig.h>

typedef union pivot {
    int i;
    double d;
} Pivot;

// Прототипы

PathsContain* sort_paths(PathsContain* path, Priority priotity);

#endif
