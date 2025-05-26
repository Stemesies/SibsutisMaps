#include <libmaps/graph.h>

int main() {
  HASH *table = hashtab_create();
  // hashtab_add(table, "Новосибирск");
  printf("%u\n", ELFHash("Pikhtovka"));
  FILE *fp = fopen("c/input", "r");
  GRAPH *graph = graph_create(HASHTAB_SIZE);
  graph_init(graph, table, fp);
  printf("%d\n", hashtab_lookup(table, "Pikhtovka"));
  show_graph(graph->n_verticles, graph->graph_matrix);
  printf("path: %d\nspeed: %d\n",
         graph->graph_matrix[ELFHash("Kolyvan")][ELFHash("Pikhtovka")].len,
         graph->graph_matrix[ELFHash("Pikhtovka")][ELFHash("Kolyvan")].speed);

  return 0;
}
