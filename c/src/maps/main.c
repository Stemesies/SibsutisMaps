#include <libmaps/mapconfig.h>
#include <maps/maps.h>

int main(int argc, char* argv[])
{
    MapConfig* config = config_create();

    if (config == NULL) {
        printf("[ОШИБКА] Невозможно выделить память под конфиг.\n");
        printf("Недостаточно ОЗУ для работы программы.\n.");
        return -1;
    }

    parse_arguments(config, argc, argv);

    if(list_size(config->points) < 2) {
        printf("Нечего искать.\n");
        printf("Введите как минимум 2 точки: откуда и куда\n");
        config_destroy(config);
        return 0;
    }

    construct_paths(config);

    config_destroy(config);

    return 0;
}