#include <ctest.h>
#include <libmaps/paths.h>
#define SRC 5
#define NUM 0
#define SPEED 70
#define LEN 79

CTEST(memory_test, node)
{
    NODE* a = def_node_construct(SRC);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(a->edge);
    ASSERT_EQUAL(SRC, a->num);

    destroy_node(a);
}

CTEST(memory_test, list)
{
    LIST* a = def_list_construct(SRC);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(a->head);
    ASSERT_NOT_NULL(a->head->edge);
    ASSERT_EQUAL(SRC, a->head->num);

    destroy_list(a);
}

CTEST(memory_test, queue)
{
    QUEUE* a = queue_create();
    ASSERT_NOT_NULL(a);
    ASSERT_EQUAL(0, a->size);

    destroy_queue(a);
}

CTEST(memory_test, paths)
{
    PATHS* a = def_path_construct();
    ASSERT_NOT_NULL(a);
    ASSERT_EQUAL(0, a->count);

    destroy_paths(a);
}

CTEST(test_list, insert)
{
    LIST* a = def_list_construct(NUM);
    int num = SRC;
    EDGE edge = {SPEED, LEN};

    insert_in_list(a, num, &edge);
    ASSERT_NOT_NULL(a->tail);
    ASSERT_NOT_NULL(a->head->edge);
    ASSERT_EQUAL(edge.len, a->head->next->edge->len);
    ASSERT_EQUAL(edge.speed, a->head->next->edge->speed);

    destroy_list(a);
}

CTEST(test_paths, insert)
{
    PATHS* paths = def_path_construct();
    LIST* path = def_list_construct(SRC);
    insert_in_path(paths, path);
    ASSERT_EQUAL(paths->first->head->num, path->head->num);
    ASSERT_EQUAL(1, paths->count);
    ASSERT_EQUAL(paths->last->head->num, path->head->num);

    destroy_paths(paths);
    destroy_list(path);
}

CTEST(test_copy, list)
{
    LIST* a = def_list_construct(SRC);
    LIST* b = copy_list(a, SRC);
    ASSERT_EQUAL(a->head->num, b->head->num);

    destroy_list(a);
    destroy_list(b);
}

CTEST(test_copy_insert, list)
{
    LIST* a = def_list_construct(SRC);
    EDGE edge = {SPEED, LEN};
    insert_in_list(a, NUM, &edge);
    LIST* b = copy_list(a, SRC);
    ASSERT_EQUAL(a->head->num, b->head->num);
    ASSERT_NOT_NULL(a->tail);
    ASSERT_NULL(b->tail);

    destroy_list(a);
    destroy_list(b);
}

CTEST(test_compare, lists)
{
    LIST* a = def_list_construct(SRC);
    EDGE edge_1 = {SPEED, LEN};
    EDGE edge_2 = {60, 3};
    EDGE edge_3 = {50, 95};
    insert_in_list(a, NUM, &edge_1);
    insert_in_list(a, 21, &edge_3);
    LIST* b = copy_list(a, SRC);
    insert_in_list(b, 9, &edge_2);
    ASSERT_EQUAL(2, compare_paths(a, b));

    destroy_list(a);
    destroy_list(b);
}

CTEST(test_visited, list)
{
    LIST* a = def_list_construct(SRC);
    EDGE edge_1 = {SPEED, LEN};
    EDGE edge_2 = {60, 3};
    EDGE edge_3 = {50, 95};
    insert_in_list(a, NUM, &edge_1);
    insert_in_list(a, 21, &edge_3);
    insert_in_list(a, 9, &edge_2);
    ASSERT_EQUAL(true, is_visited(a, SRC));
    ASSERT_EQUAL(true, is_visited(a, 21));
    ASSERT_EQUAL(true, is_visited(a, 9));
    ASSERT_NOT_EQUAL(true, is_visited(a, 15));

    destroy_list(a);
}

CTEST(test_add, queue)
{
    QUEUE* a = queue_create();
    EDGE edge = {SPEED, LEN};
    queue_add(a, NUM, &edge);
    ASSERT_EQUAL(NUM, a->head->num);
    ASSERT_NOT_NULL(a->tail);
    ASSERT_EQUAL(1, a->size);

    destroy_queue(a);
}

CTEST(test_take, queue)
{
    QUEUE* a = queue_create();
    EDGE edge = {SPEED, LEN};
    queue_add(a, NUM, &edge);
    NODE* taken = queue_take(a);
    ASSERT_EQUAL(NUM, taken->num);
    ASSERT_EQUAL(0, a->size);

    destroy_queue(a);
    destroy_node(taken);
}
