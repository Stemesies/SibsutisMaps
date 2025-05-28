#ifndef MAPCONFIG_H_
#define MAPCONFIG_H_

#include <libmaps/graph.h>
#include <libmaps/list.h>

#define DEFAULT_OUTPUT_PATH "result"

typedef enum priority { SHORTEST, QUICKEST, LONGEST } Priority;

/*
Настройки данной программы.

** priority: кратчайший/быстрейший/длиннейший приоритет программы
** limit: ограничение кол-ва промежуточных точек при нахождении пути
(для отключения должен быть меньше 3)
** altWaysCount: количество дополнительных путей, предлагаемых программой
** altWaysFilterCoefficient: коэффициент оптимальности альтернативных путей

** outputStream: путь к файлу, в который будет выводиться результат работы
программы. Если "null" - программа должна все выводить в консоль
** moreDetailedOutput: более подробная визуальная маршрута.
*/
typedef struct config {
    List* points;

    Priority priority;
    unsigned int limit;
    unsigned int altways_count;
    float altways_filter_coefficient;

    char* output_stream;
    char more_detailed_output;

} MapConfig;

MapConfig* config_create();
void config_destroy(MapConfig* config);
void config_init(MapConfig* config);

/*
Сравнивает поданые конфиги, возвращая 1 если они идентичны, 0 - если нет.
*/
bool config_compare(MapConfig* config1, MapConfig* config2);

/*
Данный метод чисто для дебага. Возможно вскоре будет удален.
*/
void config_print(MapConfig* config);

/*
Обрабатывает аргументы, поданные при запуске программы
Составляет конфиг.
*/
void parse_arguments(MapConfig* config, int argc, char* argv[]);

#endif
