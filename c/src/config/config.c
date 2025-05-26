#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Config config;

#define isflag(str) strcmp(argv[i], str) == 0
#define isflag2(str, str2) isflag(str) || isflag(str2)
#define expect_argument(message_expected_value)                                \
    if (argc <= i + 1) {                                                       \
        printf("Флаг %s ожидает " message_expected_value " после себя.\n",     \
               argv[i]);                                                       \
        exit(-1);                                                              \
    } else if (argv[i + 1][0] == '-') {                                        \
        printf("Флаг %s ожидает " message_expected_value " после себя.\n",     \
               argv[i]);                                                       \
        printf("... Но был встречен флаг %s.\n", argv[i + 1]);                 \
        exit(-1);                                                              \
    }

void print_help() {
    printf("Тут должна быть документация по использованию программы\n");
}

void config_init(Config *config, char *default_output_path) {
    config->priority = Quickest;
    config->limit = 0;
    config->altways_count = 0;
    config->altways_filter_coefficient = 1.5;

    config->output_stream = default_output_path;
    config->more_detailed_output = 0;
}

/*
Сравнивает поданые конфиги, возвращая 1 если они идентичны, 0 - если нет.
*/
int config_compare(Config *config1, Config *config2) {
    if (config1->altways_count != config2->altways_count)
        return 0;
    if (config1->altways_filter_coefficient
        != config2->altways_filter_coefficient)
        return 0;
    if (config1->limit != config2->limit)
        return 0;
    if (config1->more_detailed_output != config2->more_detailed_output)
        return 0;
    if (config1->output_stream != config2->output_stream)
        return 0;

    return config1->priority == config2->priority;
}

/*
Данный метод чисто для дебага. Возможно вскоре будет удален.
*/
void config_print(Config *config) {
    printf("\n--- Конфиг приложения ---\n");
    printf("Приоритет: ");

    switch (config->priority) {
    case Quickest:
        puts("Быстрейший");
        break;
    case Shortest:
        puts("Кратчайший");
        break;
    case Longest:
        puts("Длиннейший");
        break;
    default:
        puts("Неизвестный");
        break;
    }

    printf("Лимит: %u\n", config->limit);
    printf("Количество альт. путей: %u\n", config->altways_count);
    printf("Коэффициент оптимальности: %f\n",
           config->altways_filter_coefficient);

    printf("Более детальный маршрут: %d\n", config->more_detailed_output);
    printf("Путь к файлу: %s\n", config->output_stream);
}

/*
Обрабатывает аргументы, поданные при запуске программы
Составляет конфиг. Вы можете обратиться к нему через
переменную config из любой точки программы.
*/
void parse_arguments(int argc, char *argv[], char *defaultOutputPath) {
    if (argc < 2) { // аргументов нет
        print_help();
        exit(0);
    }

    config_init(&config, defaultOutputPath);

    // Первый аргумент (i=0) всегда является именем файла. Пропускаем.
    for (int i = 1; i < argc; i++) {

        if (argv[i][0] != '-') {
            // тут мы должны добавить точку в лист config.points
            printf("Встречена точка %s\n", argv[i]);
        } else {

            if (isflag2("-Q", "--quickest")) {
                config.priority = Quickest;
            } else if (isflag2("-S", "--shortest")) {
                config.priority = Shortest;
            } else if (isflag2("-L", "--longest")) {
                config.priority = Longest;
            }

            else if (isflag2("-l", "--limit")) {
                expect_argument("целое число");
                // Инкрементируем i, чтобы программа пропустила аргумент.
                config.limit = atoi(argv[++i]);
            } else if (isflag2("-alts", "--show-alternatives")) {
                expect_argument("целое число");
                config.altways_count = atoi(argv[++i]);
            } else if (isflag2("-altf", "--alt-filter")) {
                expect_argument("число с плавающей точкой");
                config.altways_filter_coefficient = atof(argv[++i]);
            }

            else if (isflag2("-p", "--path")) {
                config.more_detailed_output = 1;
            } else if (isflag2("-f", "--print-to-file")) {
                expect_argument("путь к файлу");
                config.output_stream = argv[++i];
            } else if (isflag2("-h", "--help")) {
                print_help();
                exit(0);
            } else {
                printf("Неизвестный флаг \"%s\".\n", argv[i]);
                exit(-1);
            }
        }
    }
}