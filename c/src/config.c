#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Config config;

#define isFlag(str) strcmp(argv[i], str) == 0
#define isFlag2(str, str2) isFlag(str) || isFlag(str2)
#define expectArgument(message_expectedValue) \
    if(argc <= i+1) { \
        printf("Флаг %s ожидает "message_expectedValue" после себя.\n", argv[i]); \
        exit(-1); \
    }  else if(argv[i+1][0] == '-') { \
        printf("Флаг %s ожидает "message_expectedValue" после себя.\n", argv[i]); \
        printf("... Но был встречен флаг %s.\n", argv[i+1]); \
        exit(-1); \
    }

void printHelp() {
    printf("Тут должна быть документация по использованию программы\n");
}

void configInit(char* defaultOutputPath) {
    config.priority = Quickest;
    config.limit = 0;
    config.altWaysCount = 0;
    config.altWaysFilterCoefficient = 1.5;

    config.outputStream = defaultOutputPath;
    config.moreDetailedOutput = 0;

}

/*
Данный метод чисто для дебага. Возможно вскоре будет удален.
*/
void configPrint() {
    printf("\n--- Конфиг приложения ---\n");
    printf("Приоритет: ");

    switch(config.priority) {
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

    printf("Лимит: %u\n", config.limit);
    printf("Количество альт. путей: %u\n", config.altWaysCount);
    printf("Коэффициент оптимальности: %f\n", config.altWaysFilterCoefficient);
   
    printf("Более детальный маршрут: %d\n", config.moreDetailedOutput);
    printf("Путь к файлу: %s\n", config.outputStream);
}

/*
Обрабатывает аргументы, поданные при запуске программы
Составляет конфиг. Вы можете обратиться к нему через
переменную config.
*/
void parseArguments(int argc, char* argv[], char* defaultOutputPath) {
    if(argc < 2) {//аргументов нет
        printHelp();
        exit(0);
    }

    configInit(defaultOutputPath);

    // Первый аргумент (i=0) всегда является именем файла. Пропускаем.
    for (int i = 1; i < argc; i++) {

        if(argv[i][0] != '-') {
            //тут мы должны добавить точку в лист config.points
            printf("Встречена точка %s\n", argv[i]);
        } else {
            
            if(isFlag2("-Q", "--quickest")) {
                config.priority = Quickest;
            } else if(isFlag2("-S", "--shortest")) {
                config.priority = Shortest;
            } else if(isFlag2("-L", "--longest")) {
                config.priority = Longest;
            }

            else if(isFlag2("-l", "--limit")) {
                expectArgument("целое число");
                // Инкрементируем i, чтобы программа пропустила аргумент.
                config.limit = atoi(argv[++i]);
            } else if(isFlag2("-alts", "--show-alternatives")) {
                expectArgument("целое число");
                config.altWaysCount = atoi(argv[++i]);
            } else if(isFlag2("-altf", "--alt-filter")) {
                expectArgument("число с плавающей точкой");
                config.altWaysFilterCoefficient = atof(argv[++i]);
            }

            else if(isFlag2("-p", "--path")) {
                config.moreDetailedOutput = 1;
            } else if(isFlag2("-f", "--print-to-file")) {
                expectArgument("путь к файлу");
                config.outputStream = argv[++i];
            } else if(isFlag2("-h", "--help")) {
                printHelp();
                exit(0);
            } else {
                printf("Неизвестный флаг \"%s\".\n", argv[i]);
                exit(-1);
            }


        }
    }
}