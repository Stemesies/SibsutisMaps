#include <libmaps/mapconfig.h>

#define isflag(str) strcmp(argv[i], str) == 0
#define isflag2(str, str2) isflag(str) || isflag(str2)
#define expect_argument(message_expected_value)                            \
    if (argc <= i + 1) {                                                   \
        printf("Флаг %s ожидает " message_expected_value " после себя.\n", \
               argv[i]);                                                   \
        exit(-1);                                                          \
    } else if (argv[i + 1][0] == '-') {                                    \
        printf("Флаг %s ожидает " message_expected_value " после себя.\n", \
               argv[i]);                                                   \
        printf("... Но был встречен флаг %s.\n", argv[i + 1]);             \
        exit(-1);                                                          \
    }

void print_help()
{
    printf("Тут должна быть документация по использованию программы\n");
}

MapConfig* config_create()
{
    MapConfig* mapconfig = (MapConfig*)malloc(sizeof(MapConfig));
    if (mapconfig == NULL)
        return NULL;
    mapconfig->points = list_create();
    if (mapconfig->points == NULL) {
        free(mapconfig);
        return NULL;
    }
    config_init(mapconfig);
    return mapconfig;
}

void config_destroy(MapConfig* config)
{
    if (config != NULL) {
        list_free(config->points);
    }
    free(config);
}

void config_init(MapConfig* config)
{
    list_clear(config->points);

    config->priority = QUICKEST;
    config->limit = 0;
    config->altways_count = 0;
    config->altways_filter_coefficient = 1.5;

    config->output_stream = DEFAULT_OUTPUT_PATH;
    config->more_detailed_output = 0;
}

bool config_compare(MapConfig* config1, MapConfig* config2)
{
    if (config1->altways_count != config2->altways_count)
        return false;
    if (config1->altways_filter_coefficient
        != config2->altways_filter_coefficient)
        return false;
    if (config1->limit != config2->limit)
        return false;
    if (config1->more_detailed_output != config2->more_detailed_output)
        return false;
    if (config1->output_stream != config2->output_stream)
        return false;

    return config1->priority == config2->priority;
}

void config_print(MapConfig* config)
{
    printf("\n--- Конфиг приложения ---\n");

    printf("Путь: ");
    list_foreach_inlined(config->points, { printf("%s -> ", list_itp(char)); });
    putchar('\n');

    printf("Приоритет: ");

    switch (config->priority) {
    case QUICKEST:
        puts("Быстрейший");
        break;
    case SHORTEST:
        puts("Кратчайший");
        break;
    case LONGEST:
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

void parse_arguments(MapConfig* config, int argc, char* argv[])
{
    if (argc < 2) { // аргументов нет
        print_help();
        config_destroy(config);
        exit(0);
    }

    config_init(config);

    // Первый аргумент (i=0) всегда является именем файла. Пропускаем.
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            list_bpush(config->points, argv[i]);
        } else {
            if (isflag2("-Q", "--quickest")) {
                config->priority = QUICKEST;
            } else if (isflag2("-S", "--shortest")) {
                config->priority = SHORTEST;
            } else if (isflag2("-L", "--longest")) {
                config->priority = LONGEST;
            }

            else if (isflag2("-l", "--limit")) {
                expect_argument("целое число");
                // Инкрементируем i, чтобы программа пропустила аргумент.
                config->limit = atoi(argv[++i]);
            } else if (isflag2("-alts", "--show-alternatives")) {
                expect_argument("целое число");
                config->altways_count = atoi(argv[++i]);
            } else if (isflag2("-altf", "--alt-filter")) {
                expect_argument("число с плавающей точкой");
                config->altways_filter_coefficient = atof(argv[++i]);
            }

            else if (isflag2("-p", "--path")) {
                config->more_detailed_output = 1;
            } else if (isflag2("-f", "--print-to-file")) {
                expect_argument("путь к файлу");
                config->output_stream = argv[++i];
            } else if (isflag2("-h", "--help")) {
                print_help();
                config_destroy(config);
                exit(0);
            } else {
                printf("Неизвестный флаг \"%s\".\n", argv[i]);
                exit(-1);
            }
        }
    }
}
