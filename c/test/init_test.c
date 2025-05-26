#include <ctest.h>
#include <libmaps/graph.h>
#include <mapconfig/mapconfig.h>

CTEST(test_graph, create)
{
    GRAPH* graph = graph_create(HASHTAB_SIZE);
    ASSERT_NOT_NULL(graph);
    ASSERT_NOT_NULL(graph->graph_matrix);
    ASSERT_NOT_NULL(graph->verticles);
    ASSERT_NOT_NULL(graph->visited);

    graph_destroy(graph);
}

CTEST(hash_table_test, create)
{
    HASH* table = hashtab_create();
    ASSERT_NOT_NULL(table);
    ASSERT_NULL(table[0].key);

    hashtab_destroy(table);
}

CTEST(test_hash, add_lookup)
{
    HASH* table = hashtab_create();
    ASSERT_EQUAL(-1, hashtab_lookup(table, "Novosibirsk"));
    ASSERT_NOT_EQUAL(true, is_in_table(table, "Novosibirsk"));
    unsigned int v = hashtab_add(table, "Novosibirsk");
    ASSERT_EQUAL(v, ELFHash("Novosibirsk"));
    ASSERT_EQUAL(ELFHash("Novosibirsk"), hashtab_lookup(table, "Novosibirsk"));
    ASSERT_EQUAL(true, is_in_table(table, "Novosibirsk"));

    hashtab_destroy(table);
}

CTEST(test_graph, add)
{
    int len = 15, speed = 55;
    GRAPH* graph = graph_create(HASHTAB_SIZE);
    add_edge(graph, ELFHash("Novosibirsk"), ELFHash("Kolyvan"), len, speed);
    ASSERT_EQUAL(
            len,
            graph->graph_matrix[ELFHash("Novosibirsk")][ELFHash("Kolyvan")]
                    .len);
    ASSERT_EQUAL(
            speed,
            graph->graph_matrix[ELFHash("Novosibirsk")][ELFHash("Kolyvan")]
                    .speed);
    ASSERT_EQUAL(
            graph->graph_matrix[ELFHash("Kolyvan")][ELFHash("Novosibirsk")].len,
            graph->graph_matrix[ELFHash("Novosibirsk")][ELFHash("Kolyvan")]
                    .len);

    graph_destroy(graph);
}

CTEST(test_hash, lookup)
{
}

CTEST(test_config, quckest_short)
{
    MapConfig cfg = {};
    int argc = 2;
    char* argv[] = {"./test", "-Q"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.priority, Quickest);
}

CTEST(test_config, shortest_full)
{
    MapConfig cfg = {};
    int argc = 2;
    char* argv[] = {"./test", "--shortest"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.priority, Shortest);
}

CTEST(test_config, altways_count_full)
{
    MapConfig cfg = {};
    int argc = 3;
    char* argv[] = {"./test", "--show-alternatives", "10"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.altways_count, 10);
}

CTEST(test_config, altway_coeff_short)
{
    MapConfig cfg = {};
    int argc = 3;
    char* argv[] = {"./test", "-altf", "0.3"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_EQUAL(cfg.altways_filter_coefficient, 0.3);
}

CTEST(test_config, output_short)
{
    MapConfig cfg = {};
    int argc = 3;
    char* argv[] = {"./test", "-f", "null"};
    parse_arguments(&cfg, argc, argv, "result");
    ASSERT_STR(cfg.output_stream, "null");
}

CTEST(test_config, set_of_arguments)
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