#include <ctest.h>
#include <libmaps/paths.h>

CTEST(memory_test, list)
{
    LIST* a = def_list_construct(5);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(a->head);
    ASSERT_NOT_NULL(a->head->edge);

    destroy_list(a);
}

CTEST(memory_test, queue)
{
    QUEUE* a = queue_create(5);
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
