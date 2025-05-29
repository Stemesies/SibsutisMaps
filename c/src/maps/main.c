#include <libmaps/mapconfig.h>
#include <maps/maps.h>

void validate_config(MapConfig* config)
{
    if (list_size(config->points) < 2) {
        printf("Нечего искать.\n");
        printf("Введите как минимум 2 точки: откуда и куда\n");
        config_destroy(config);
        exit(EXIT_SUCCESS);
    }

    if (config->limit > 0 && config->limit < config->points->size) {
        printf("Невозможно найти путь, состоящий из максимум %d точек\n",
               config->limit);
        printf("Но проходящий через %d точек.\n", config->points->size);
        config_destroy(config);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[])
{
    MapConfig* config = config_create();

    if (config == NULL) {
        printf("[ОШИБКА] Невозможно выделить память под конфиг.\n");
        printf("Недостаточно ОЗУ для работы программы.\n.");
        return -1;
    }

    parse_arguments(config, argc, argv);
    validate_config(config);

    construct_paths(config);

    config_destroy(config);
    return 0;
}