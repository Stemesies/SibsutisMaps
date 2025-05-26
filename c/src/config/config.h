typedef enum PRIORITY { Shortest, Quickest, Longest } Priority;

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
typedef struct CONFIG {
    // List* points (Начальная, конечная и промежуточные точки)

    Priority priority;
    unsigned int limit;
    unsigned int altways_count;
    float altways_filter_coefficient;

    char *output_stream;
    char more_detailed_output;

} Config;

extern Config config;

void config_init(char *default_output_path);

/*
Данный метод чисто для дебага. Возможно вскоре будет удален.
*/
void config_print();

/*
Обрабатывает аргументы, поданные при запуске программы
Составляет конфиг. Вы можете обратиться к нему через
переменную config.
*/
void parse_arguments(int argc, char *argv[], char *defaultOutputPath);