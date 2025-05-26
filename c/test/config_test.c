#include <ctest.h>
#include <mapconfig/mapconfig.h>

CTEST(config, quckest_short)
{
    MapConfig cfg = {};
    int argc = 2;
    char* argv[] = {"./test", "-Q"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.priority, Quickest);
}

CTEST(config, shortest_full)
{
    MapConfig cfg = {};
    int argc = 2;
    char* argv[] = {"./test", "--shortest"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.priority, Shortest);
}

CTEST(config, altways_count_full)
{
    MapConfig cfg = {};
    int argc = 3;
    char* argv[] = {"./test", "--show-alternatives", "10"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.altways_count, 10);
}

CTEST(config, altway_coeff_short)
{
    MapConfig cfg = {};
    int argc = 3;
    char* argv[] = {"./test", "-altf", "0.3"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.altways_filter_coefficient, 0.3);
}

CTEST(config, output_short)
{
    MapConfig cfg = {};
    int argc = 3;
    char* argv[] = {"./test", "-f", "null"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_STR(cfg.output_stream, "null");
}

CTEST(config, set_of_arguments)
{
    MapConfig cfg = {};
    int argc = 11;
    char* argv[]
            = {"./test",
               "--path",
               "-alts",
               "3",
               "-alts",
               "4",
               "-L",
               "-l",
               "10",
               "-f",
               "output.txt"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.altways_count, 4);
    ASSERT_EQUAL(cfg.limit, 10);
    ASSERT_EQUAL(cfg.more_detailed_output, 1);
    ASSERT_EQUAL(cfg.priority, Longest);
    ASSERT_STR(cfg.output_stream, "output.txt");
}
