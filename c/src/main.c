#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

void amogus() { printf("exit\n"); }

int main(int argc, char *argv[]) {
    // Обработка аргументов и записывание настроек в структуру Config.
    char *defaultOutputPath = "result";
    parseArguments(argc, argv, defaultOutputPath);

    configPrint();

    return 0;
}