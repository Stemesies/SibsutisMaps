#include <ctest.h>
#include <mapconfig/mapconfig.h>

CTEST(config, config_create)
{
    MapConfig* cfg = config_create();
    ASSERT_NOT_NULL(cfg);
    ASSERT_NOT_NULL(cfg->points);
    config_dispose(cfg);
}

CTEST(config, quckest_short)
{
    MapConfig* cfg = config_create();
    int argc = 2;
    char* argv[] = {"./test", "-Q"};
    parse_arguments(cfg, argc, argv);
    ASSERT_EQUAL(cfg->priority, Quickest);
    config_dispose(cfg);
}

CTEST(config, shortest_full)
{
    MapConfig* cfg = config_create();
    int argc = 2;
    char* argv[] = {"./test", "--shortest"};
    parse_arguments(cfg, argc, argv);
    ASSERT_EQUAL(cfg->priority, Shortest);
    config_dispose(cfg);
}

CTEST(config, altways_count_full)
{
    MapConfig* cfg = config_create();
    int argc = 3;
    char* argv[] = {"./test", "--show-alternatives", "10"};
    parse_arguments(cfg, argc, argv);
    ASSERT_EQUAL(cfg->altways_count, 10);
    config_dispose(cfg);
}

CTEST(config, altway_coeff_short)
{
    MapConfig* cfg = config_create();
    int argc = 3;
    char* argv[] = {"./test", "-altf", "0.3"};
    parse_arguments(cfg, argc, argv);
    ASSERT_EQUAL(cfg->altways_filter_coefficient, 0.3);
    config_dispose(cfg);
}

CTEST(config, output_short)
{
    MapConfig* cfg = config_create();
    int argc = 3;
    char* argv[] = {"./test", "-f", "null"};
    parse_arguments(cfg, argc, argv);
    ASSERT_STR(cfg->output_stream, "null");
    config_dispose(cfg);
}

CTEST(config, points)
{
    MapConfig* cfg = config_create();
    int argc = 3;
    char* argv[] = {"./test", "Novosibirsk", "Chulym"};
    parse_arguments(cfg, argc, argv);
    ASSERT_NOT_NULL(cfg->points);
    ASSERT_NOT_NULL(cfg->points->head);

    ASSERT_STR(list_getof(char, cfg->points, 0), "Novosibirsk");
    ASSERT_STR(list_getof(char, cfg->points, 1), "Chulym");
    config_dispose(cfg);
}

CTEST(config, set_of_arguments)
{
    MapConfig* cfg = config_create();
    int argc = 16;
    char* argv[]
            = {"./test",
               "--path",
               "Novosibirsk",
               "Iskitim",
               "-alts",
               "3",
               "Dovolnoe",
               "-alts",
               "4",
               "Novosibirsk",
               "-L",
               "-l",
               "10",
               "-f",
               "output.txt",
               "Kolyvan"};
    parse_arguments(cfg, argc, argv);
    ASSERT_EQUAL(cfg->altways_count, 4);
    ASSERT_EQUAL(cfg->limit, 10);
    ASSERT_EQUAL(cfg->more_detailed_output, 1);
    ASSERT_EQUAL(cfg->priority, Longest);
    ASSERT_STR(cfg->output_stream, "output.txt");

    ASSERT_NOT_NULL(cfg->points);
    ASSERT_NOT_NULL(cfg->points->head);

    ASSERT_STR(list_getof(char, cfg->points, 0), "Novosibirsk");
    ASSERT_STR(list_getof(char, cfg->points, 1), "Iskitim");
    ASSERT_STR(list_getof(char, cfg->points, 2), "Dovolnoe");
    ASSERT_STR(list_getof(char, cfg->points, 3), "Novosibirsk");
    ASSERT_STR(list_getof(char, cfg->points, 4), "Kolyvan");
    config_dispose(cfg);
}
