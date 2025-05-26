#include "config.h"
#include <stdio.h>

int _sibsutismaps_test_for(int argc, char *argv[], Config *expected) {
    parse_arguments(argc, argv, "result");

    printf("Тест с аргументами [ ");
    for (int i = 1; i < argc; i++)
        printf("%s ", argv[i]);
    putchar(']');

    if (config_compare(&config, expected)) {
        printf(" пройден \n");
        return 1;
    } else {
        printf(" провален \n");
        printf("\nОжидаемый конфиг:\n");
        config_print(expected);
        printf("\nПолученный конфиг:\n");
        config_print(&config);
        return 0;
    }
}

#define reset_expected_config() config_init(&expected, "result");

#define test_for(argc, ...)                                                    \
    if (1) {                                                                   \
        char *atgvf[argc] = __VA_ARGS__;                                       \
        int success = _sibsutismaps_test_for(argc, atgvf, &expected);          \
        if (!success)                                                          \
            passed = 0;                                                        \
    }

int main() {
    printf("Unit-Тестирование системы аргументов\n");

    Config expected = {};
    int passed = 1;

    reset_expected_config();
    expected.priority = Quickest;
    test_for(2, {"./test", "-Q"});

    reset_expected_config();
    expected.priority = Shortest;
    expected.altways_count = 10;
    test_for(4, {"./test", "--show-alternatives", "10", "-S"});

    reset_expected_config();
    expected.altways_filter_coefficient = 0.3f;
    expected.output_stream = "null";
    test_for(5, {"./test", "-altf", "0.3", "-f", "null"});

    reset_expected_config();
    expected.more_detailed_output = 1;
    expected.altways_count = 4;
    expected.priority = Longest;
    expected.limit = 10;
    expected.output_stream = "output.txt";
    test_for(11, {"./test", "--path", "-alts", "3", "-alts", "4", "-L", "-l", "10", "-f", "output.txt"});

    if (!passed)
        printf("Программа не прошла проверку.\n");

    return !passed;
}