#include <libmaps/paths.h>

int main()
{
    PATHS* path = def_path_construct();
    GRAPH* graph = graph_create(HASHTAB_SIZE);

    graph->graph_matrix[0][1].len = 15;
    graph->graph_matrix[0][2].len = 10;
    graph->graph_matrix[0][3].len = 25;
    graph->graph_matrix[1][4].len = 26;
    graph->graph_matrix[2][4].len = 8;
    graph->graph_matrix[3][5].len = 17;
    graph->graph_matrix[4][5].len = 14;
    graph->graph_matrix[4][6].len = 19;
    graph->graph_matrix[5][6].len = 33;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++)
            if (graph->graph_matrix[i][j].len > 0)
                graph->graph_matrix[j][i].len = graph->graph_matrix[i][j].len;
    }
    show_graph(HASHTAB_SIZE, graph->graph_matrix);

    Dfs(0, 5, path, graph);
    // Bfs(0, 5, path, 7, graph);

    printf("count: %d\n", path->count);
    show_paths(path);
    graph_destroy(graph);

    return 0;
}
