#include <libmaps/mapconfig.h>
#include <maps/maps.h>

int main(int argc, char* argv[])
{
    mapconfig = config_create();

    if (mapconfig == NULL) {
        printf("[ОШИБКА] Невозможно выделить память под конфиг.\n");
        printf("Недостаточно ОЗУ для работы программы.\n.");
        return -1;
    }

    parse_arguments(mapconfig, argc, argv);

    construct_paths();

    return 0;
}