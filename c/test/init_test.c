#include <ctest.h>
#include <libmaps/graph.h>

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
