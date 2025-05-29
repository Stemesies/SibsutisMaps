#include <ctest.h>
#include <libmaps/graph.h>
#define N_VERT 82

CTEST(test_graph, create)
{
    Graph* graph = graph_create(HASHTABSIZE);
    ASSERT_NOT_NULL(graph);
    ASSERT_NOT_NULL(graph->graph_matrix);
    ASSERT_NOT_NULL(graph->verticles);
    ASSERT_NOT_NULL(graph->visited);

    graph_destroy(graph);
}

CTEST(hash_table_test, create)
{
    HashTable* table = hashtab_create();
    ASSERT_NOT_NULL(table);
    ASSERT_NULL(table[0].key);
    ASSERT_EQUAL(0, table->count);

    hashtab_destroy(table);
}

CTEST(test_hash, add_lookup)
{
    HashTable* table = hashtab_create();
    ASSERT_EQUAL(-1, hashtab_lookup(table, "Novosibirsk"));
    ASSERT_NOT_EQUAL(true, is_in_table(table, "Novosibirsk"));
    unsigned int v = hashtab_add(table, "Novosibirsk");
    ASSERT_EQUAL(v, ELFHash("Novosibirsk"));
    ASSERT_EQUAL(ELFHash("Novosibirsk"), hashtab_lookup(table, "Novosibirsk"));
    ASSERT_EQUAL(true, is_in_table(table, "Novosibirsk"));
    ASSERT_NOT_EQUAL(0, table->count);

    hashtab_destroy(table);
}

CTEST(test_graph, add)
{
    int len = 15, speed = 55;
    Graph* graph = graph_create(HASHTABSIZE);
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

CTEST(test_graph, init)
{
    Graph* graph = graph_create(HASHTABSIZE);
    HashTable* table = hashtab_create();
    FILE* fp = fopen("input", "r");
    ASSERT_NOT_NULL(fp);
    graph_init(graph, table, fp);
    fseek(fp, 0, SEEK_SET);
    char* str = calloc(MAXSTR, sizeof(char));
    char ch;
    int count = 0;
    /*Считываем первый нас. пункт, чтобы проверить, есть ли он в таблице*/
    while ((ch = fgetc(fp)) != ' ' && ch != EOF) {
        str[count] = ch;
        count++;
    }
    str[count] = '\0';
    ASSERT_EQUAL(true, is_in_table(table, str));
    ASSERT_EQUAL(N_VERT, table->count);

    graph_destroy(graph);
    hashtab_destroy(table);
    free(str);
    fclose(fp);
}

CTEST(test_in, empty_input)
{
    FILE* fp = fopen("c/test/empty_input", "r");
    HashTable* table = hashtab_create();
    Graph* graph = graph_create(1);
    graph_init(graph, table, fp);
    ASSERT_EQUAL(0, table->count);
    show_graph(1, graph->graph_matrix);

    graph_destroy(graph);
    hashtab_destroy(table);
    fclose(fp);
}
