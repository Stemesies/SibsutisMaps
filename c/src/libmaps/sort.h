#ifndef SORT_H_
#define SORT_H

#include <libmaps/mapconfig.h>
#include <libmaps/paths.h>

typedef union pivot {
    int i;
    double d;
} Pivot;

// Прототипы

/*Сортирует пути по длине или времени преодоления, учитывая выбранный приоритет.
 * Помещает отсортированные пути в новый контейнер (старый при этом не
 * очищается!)*/
PathsContain* sort_paths(PathsContain* path, Priority priotity);

#endif
