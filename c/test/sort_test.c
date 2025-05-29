#include <ctest.h>
#include <libmaps/search.h>

CTEST(test_correct_paths, validate)
{
    PathsContain* path = def_path_contain_construct();
    Graph* graph = graph_create(7);

    graph->graph_matrix[0][1].len = 15;
    graph->graph_matrix[0][2].len = 10;
    graph->graph_matrix[0][3].len = 25;
    graph->graph_matrix[1][4].len = 26;
    graph->graph_matrix[2][4].len = 8;
    graph->graph_matrix[3][5].len = 17;
    graph->graph_matrix[4][5].len = 14;
    graph->graph_matrix[4][6].len = 19;
    graph->graph_matrix[5][6].len = 33;

    graph->graph_matrix[0][1].speed = 80;
    graph->graph_matrix[0][2].speed = 60;
    graph->graph_matrix[0][3].speed = 85;
    graph->graph_matrix[1][4].speed = 60;
    graph->graph_matrix[2][4].speed = 58;
    graph->graph_matrix[3][5].speed = 37;
    graph->graph_matrix[4][5].speed = 69;
    graph->graph_matrix[4][6].speed = 73;
    graph->graph_matrix[5][6].speed = 90;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++)
            if (graph->graph_matrix[i][j].len > 0) {
                graph->graph_matrix[j][i].len = graph->graph_matrix[i][j].len;
                graph->graph_matrix[j][i].speed
                        = graph->graph_matrix[i][j].speed;
            }
    }
    Map map = {graph, NULL};
    MapConfig config = {.limit = 0};
    SearchContext context
            = {.map = &map,
               .src = 0,
               .res = 5,
               .paths = path,
               .config = &config,
               0};

    Path* supp = def_path_construct(0);
    Dfs(&context, 0, supp);

    ASSERT_EQUAL(5, context.paths->first->tail->num);
    ASSERT_EQUAL(5, context.paths->last->tail->num);
    ASSERT_EQUAL(5, context.paths->first->next->tail->num);

    graph_destroy(graph);
    destroy_paths_contain(context.paths);
    free(supp);
}

CTEST(test_sort, validate)
{
    PathsContain* path = def_path_contain_construct();
    Graph* graph = graph_create(7);

    graph->graph_matrix[0][1].len = 15;
    graph->graph_matrix[0][2].len = 10;
    graph->graph_matrix[0][3].len = 25;
    graph->graph_matrix[1][4].len = 26;
    graph->graph_matrix[2][4].len = 8;
    graph->graph_matrix[3][5].len = 17;
    graph->graph_matrix[4][5].len = 14;
    graph->graph_matrix[4][6].len = 19;
    graph->graph_matrix[5][6].len = 33;

    graph->graph_matrix[0][1].speed = 80;
    graph->graph_matrix[0][2].speed = 60;
    graph->graph_matrix[0][3].speed = 85;
    graph->graph_matrix[1][4].speed = 60;
    graph->graph_matrix[2][4].speed = 58;
    graph->graph_matrix[3][5].speed = 37;
    graph->graph_matrix[4][5].speed = 69;
    graph->graph_matrix[4][6].speed = 73;
    graph->graph_matrix[5][6].speed = 90;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++)
            if (graph->graph_matrix[i][j].len > 0) {
                graph->graph_matrix[j][i].len = graph->graph_matrix[i][j].len;
                graph->graph_matrix[j][i].speed
                        = graph->graph_matrix[i][j].speed;
            }
    }

    Map map = {graph, NULL};
    MapConfig config = {.limit = 0};
    SearchContext context
            = {.map = &map,
               .src = 0,
               .res = 5,
               .paths = path,
               .config = &config,
               0};

    Path* supp = def_path_construct(0);
    Dfs(&context, 0, supp);

    PathsContain* sorted_paths1 = sort_paths(context.paths, SHORTEST);
    ASSERT_EQUAL(32, sorted_paths1->first->path);

    PathsContain* sorted_paths2 = sort_paths(sorted_paths1, LONGEST);
    ASSERT_EQUAL(93, sorted_paths2->first->path);

    PathsContain* sorted_paths3 = sort_paths(sorted_paths2, QUICKEST);
    ASSERT_DBL_NEAR_TOL(0.5, sorted_paths3->first->time, 0.1);

    graph_destroy(graph);
    destroy_paths_contain(context.paths);
    destroy_paths_contain(sorted_paths1);
    destroy_paths_contain(sorted_paths2);
    destroy_paths_contain(sorted_paths3);
    free(supp);
}
