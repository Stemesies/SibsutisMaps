#ifndef SRCH_H_
#define SRCH_H_

#include <libmaps/graph.h>
#include <libmaps/mapconfig.h>
#include <libmaps/paths.h>
#include <libmaps/sort.h>

/*
Map* map - структура, хранящая граф и хэш-таблицу
PathsContain* paths - найденные пути
MapConfig* config - Конфигурация приложения
int src - точка отправления
int res - точка прибытия
int* input_points - массив из айдиншиков введеных пользователем населенных
пунктов int input_points_size - сколько айдишников в данном массиве
*/
typedef struct search_context {
    Map* map;
    PathsContain* paths;
    MapConfig* config;
    int src;
    int res;
    int* input_points;
    int input_points_size;
} SearchContext;

/*Модифицированных обход в глубину. Находит и записывает в контейнер путей все
 *Подходящие под фильтры пути*/
void Dfs(SearchContext* context, int src, Path* current_path);

/*Ищет лучший путь по выбранному приоритету*/
Path* best_path(PathsContain* paths);

/*Ищет альтернативные пути, в rate раз медленнее/дольше/короче лучшего*/
void alternative(SearchContext* context);

/*Поиск всех путей (вызывает Dfs)*/
PathsContain* search_all_paths(SearchContext* context);

#endif
