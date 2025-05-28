#include <ctest.h>
#include <libmaps/paths.h>
#define SRC 5
#define NUM 0
#define SPEED 70
#define LEN 79

CTEST(memory_test, node)
{
    PathNode* a = def_node_construct(SRC);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(a->edge);
    ASSERT_EQUAL(SRC, a->num);

    destroy_node(a);
}

CTEST(memory_test, list)
{
    Path* a = def_path_construct(SRC);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(a->head);
    ASSERT_NOT_NULL(a->head->edge);
    ASSERT_EQUAL(SRC, a->head->num);

    destroy_path(a);
}

CTEST(memory_test, queue)
{
    Queue* a = queue_create();
    ASSERT_NOT_NULL(a);
    ASSERT_EQUAL(0, a->size);

    destroy_queue(a);
}

CTEST(memory_test, paths)
{
    PathsContain* a = def_path_contain_construct();
    ASSERT_NOT_NULL(a);
    ASSERT_EQUAL(0, a->count);

    destroy_paths_contain(a);
}

CTEST(test_list, insert)
{
    Path* a = def_path_construct(NUM);
    int num = SRC;
    Edge edge = {SPEED, LEN};

    insert_in_path(a, num, &edge);
    ASSERT_NOT_NULL(a->tail);
    ASSERT_NOT_NULL(a->head->edge);
    ASSERT_EQUAL(edge.len, a->head->next->edge->len);
    ASSERT_EQUAL(edge.speed, a->head->next->edge->speed);

    destroy_path(a);
}

CTEST(test_paths, insert)
{
    PathsContain* paths = def_path_contain_construct();
    Path* path = def_path_construct(SRC);
    insert_in_path_contain(paths, path);
    ASSERT_EQUAL(paths->first->head->num, path->head->num);
    ASSERT_EQUAL(1, paths->count);
    ASSERT_EQUAL(paths->last->head->num, path->head->num);

    destroy_paths_contain(paths);
    destroy_path(path);
}

CTEST(test_copy, list)
{
    Path* a = def_path_construct(SRC);
    Path* b = copy_path(a, SRC);
    ASSERT_EQUAL(a->head->num, b->head->num);

    destroy_path(a);
    destroy_path(b);
}

CTEST(test_copy_insert, list)
{
    Path* a = def_path_construct(SRC);
    Edge edge = {SPEED, LEN};
    insert_in_path(a, NUM, &edge);
    Path* b = copy_path(a, SRC);
    ASSERT_EQUAL(a->head->num, b->head->num);
    ASSERT_NOT_NULL(a->tail);
    ASSERT_NULL(b->tail);

    destroy_path(a);
    destroy_path(b);
}

CTEST(test_compare, lists)
{
    Path* a = def_path_construct(SRC);
    Edge edge_1 = {SPEED, LEN};
    Edge edge_2 = {60, 3};
    Edge edge_3 = {50, 95};
    insert_in_path(a, NUM, &edge_1);
    insert_in_path(a, 21, &edge_3);
    Path* b = copy_path(a, SRC);
    insert_in_path(b, 9, &edge_2);
    ASSERT_EQUAL(2, compare_paths(a, b));

    destroy_path(a);
    destroy_path(b);
}

CTEST(test_visited, list)
{
    Path* a = def_path_construct(SRC);
    Edge edge_1 = {SPEED, LEN};
    Edge edge_2 = {60, 3};
    Edge edge_3 = {50, 95};
    insert_in_path(a, NUM, &edge_1);
    insert_in_path(a, 21, &edge_3);
    insert_in_path(a, 9, &edge_2);
    ASSERT_EQUAL(true, is_visited(a, SRC));
    ASSERT_EQUAL(true, is_visited(a, 21));
    ASSERT_EQUAL(true, is_visited(a, 9));
    ASSERT_NOT_EQUAL(true, is_visited(a, 15));

    destroy_path(a);
}

CTEST(test_add, queue)
{
    Queue* a = queue_create();
    Edge edge = {SPEED, LEN};
    queue_add(a, NUM, &edge);
    ASSERT_EQUAL(NUM, a->head->num);
    ASSERT_NOT_NULL(a->tail);
    ASSERT_EQUAL(1, a->size);

    destroy_queue(a);
}

CTEST(test_take, queue)
{
    Queue* a = queue_create();
    Edge edge = {SPEED, LEN};
    queue_add(a, NUM, &edge);
    PathNode* taken = queue_take(a);
    ASSERT_EQUAL(NUM, taken->num);
    ASSERT_EQUAL(0, a->size);

    destroy_queue(a);
    destroy_node(taken);
}

CTEST(pop_test, path)
{
    Path* a = def_path_construct(SRC);
    Edge edge_1 = {SPEED, LEN};
    Edge edge_2 = {60, 3};
    Edge edge_3 = {50, 95};
    insert_in_path(a, NUM, &edge_1);
    insert_in_path(a, 21, &edge_3);
    insert_in_path(a, 9, &edge_2);

    PathNode* temp = a->head->next;
    PathNode* pop = pop_node(a);
    ASSERT_EQUAL(a->head->num, temp->num);
    ASSERT_EQUAL(SRC, pop->num);

    destroy_path(a);
    destroy_node(pop);
}

CTEST(1, 2)
{
    Path* to = def_path_construct(0);   // путь: 0->3->4->5
    Path* back = def_path_construct(9); // путь: 9->1->2->5

    Edge* edge_1 = calloc(1, sizeof(Edge));
    edge_1->len = LEN;
    edge_1->speed = SPEED;
    Edge* edge_2 = calloc(1, sizeof(Edge));
    edge_2->len = 3;
    edge_2->speed = 60;
    Edge* edge_3 = calloc(1, sizeof(Edge));
    edge_3->len = 17;
    edge_3->speed = 47;
    insert_in_path(back, 1, edge_1);
    insert_in_path(back, 2, edge_3);
    insert_in_path(back, 5, edge_2);

    Edge* edge_4 = calloc(1, sizeof(Edge));
    edge_4->len = 57;
    edge_4->speed = 69;
    Edge* edge_5 = calloc(1, sizeof(Edge));
    edge_5->len = 39;
    edge_5->speed = 68;
    Edge* edge_6 = calloc(1, sizeof(Edge));
    edge_6->len = 25;
    edge_6->speed = 29;

    insert_in_path(to, 3, edge_4);
    insert_in_path(to, 4, edge_5);
    insert_in_path(to, 5, edge_6);

    Path* res = path_with_return(to, back);
    ASSERT_EQUAL(0, res->head->num);
    ASSERT_EQUAL(9, res->tail->num);
    ASSERT_EQUAL(3, res->head->next->num);
    ASSERT_EQUAL(2, res->head->next->next->next->next->num);
    ASSERT_EQUAL(to->path + back->path, res->path);
    ASSERT_DBL_NEAR_TOL(to->time + back->time, res->time, 0.1);

    free(edge_1);
    free(edge_2);
    free(edge_3);
    free(edge_4);
    free(edge_5);
    free(edge_6);
    destroy_path(to);
    destroy_path(back);
    destroy_path(res);
}
