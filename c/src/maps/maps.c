#include <libmaps/paths.h>

int main() {
  PATHS *path = def_path_construct();
  int *verticles = calloc(7, sizeof(int));
  bool *visited = calloc(7, sizeof(bool));
  EDGE **graph = malloc(7 * sizeof(EDGE *));
  if (!graph) {
    puts("noo(");
    exit(EXIT_FAILURE);
  }
  for (int v = 0; v < 7; v++) {
    graph[v] = malloc(7 * sizeof(EDGE));
    if (!graph[v])
      exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      graph[i][j].len = 0;
      graph[i][j].speed = 0.0;
    }
  }

  graph[0][1].len = 15;
  graph[0][2].len = 10;
  graph[0][3].len = 25;
  graph[1][4].len = 26;
  graph[2][4].len = 8;
  graph[3][5].len = 17;
  graph[4][5].len = 14;
  graph[4][6].len = 19;
  graph[5][6].len = 33;

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++)
      if (graph[i][j].len > 0)
        graph[j][i].len = graph[i][j].len;
  }

  // Dfs(0, 5, path, graph);
  // Bfs(0, 5, path, 7, graph);

  // printf("count: %d\n", path->count);
  show_paths(path);
  free(verticles);
  free(visited);

  return 0;
}
