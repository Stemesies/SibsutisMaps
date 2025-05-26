#include <libmaps/graph.h>

int main()
{
    HASH* table = hashtab_create();
    printf("%u\n", ELFHash("Pikhtovka"));
    FILE* fp = fopen("c/input", "r");
    GRAPH* graph = graph_create(HASHTAB_SIZE);
    graph_init(graph, table, fp);
    printf("%d\n", hashtab_lookup(table, "Pikhtovka"));
    show_graph(graph->n_verticles, graph->graph_matrix);
    printf("path: %d\nspeed: %d\n",
           graph->graph_matrix[hashtab_lookup(table, "Kolyvan")]
                              [hashtab_lookup(table, "Pikhtovka")]
                                      .len,
           graph->graph_matrix[hashtab_lookup(table, "Pikhtovka")]
                              [hashtab_lookup(table, "Kolyvan")]
                                      .speed);

    fclose(fp);
    hashtab_destroy(table);
    graph_destroy(graph);

    return 0;
}
