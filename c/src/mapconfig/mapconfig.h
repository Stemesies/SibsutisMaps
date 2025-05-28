#ifndef MAPCONFIG_H_
#define MAPCONFIG_H_

#include <libmaps/list.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_OUTPUT_PATH "result"

typedef enum PRIORITY { SHORTEST, QUICKEST, LONGEST } Priority;

/*
Настройки данной программы.
Вы можете обратиться к ним из любой точки программы
с помощью переменной mapconfig, если подключите mapconfig.h

** priority: кратчайший/быстрейший/длиннейший приоритет программы
** limit: ограничение кол-ва промежуточных точек при нахождении пути
(для отключения должен быть меньше 3)
** altWaysCount: количество дополнительных путей, предлагаемых программой
** altWaysFilterCoefficient: коэффициент оптимальности альтернативных путей

** outputStream: путь к файлу, в который будет выводиться результат работы
программы. Если "null" - программа должна все выводить в консоль
** moreDetailedOutput: более подробная визуальная маршрута.
*/
typedef struct CONFIG {
    List* points;

    Priority priority;
    unsigned int limit;
    unsigned int altways_count;
    float altways_filter_coefficient;

    char* output_stream;
    char more_detailed_output;

} MapConfig;

extern MapConfig mapconfig;

MapConfig* config_create();
void config_dispose(MapConfig* config);
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
Составляет конфиг. Вы можете обратиться к нему через
переменную mapconfig из любой точки программы.
*/
void parse_arguments(MapConfig* config, int argc, char* argv[]);

#endif
