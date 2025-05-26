#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/config.h"

void amogus() { printf("exit\n"); }

int main(int argc, char *argv[]) {
    // Обработка аргументов и записывание настроек в структуру Config.
    char *default_output_path = "result";
    parse_arguments(argc, argv, default_output_path);

    config_print(&config);

    return 0;
}