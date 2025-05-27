#ifndef SORT_H_
#define SORT_H

#include <libmaps/paths.h>
#include <mapconfig/mapconfig.h>

typedef union pivot {
    int i;
    double d;
} Pivot;

// Прототипы

PATHS* sort_paths(PATHS* path, Priority priotity);

#endif
