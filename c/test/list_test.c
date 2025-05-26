#include <ctest.h>
#include <liblist/list.h>

CTEST(list, list_create)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);
    list_free(list);
}

CTEST(list, list_fpush)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);
    int a = 1;
    list_fpush(list, &a);
    ASSERT_EQUAL(list_size(list), 1);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);
    ASSERT_EQUAL(*((int*)list->head->value), 1);

    int b = 2;
    list_fpush(list, &b);
    ASSERT_EQUAL(list_size(list), 2);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);
    ASSERT_EQUAL(*((int*)list->head->value), 2);
    ASSERT_EQUAL(*((int*)list->tail->value), 1);

    list_free(list);
}

CTEST(list, list_bpush)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);
    int a = 1;
    list_bpush(list, &a);
    ASSERT_EQUAL(list_size(list), 1);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);
    ASSERT_EQUAL(*((int*)list->head->value), 1);

    int b = 2;
    list_bpush(list, &b);
    ASSERT_EQUAL(list_size(list), 2);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);
    ASSERT_EQUAL(*((int*)list->head->value), 1);
    ASSERT_EQUAL(*((int*)list->tail->value), 2);

    list_free(list);
}

CTEST(list, list_fpop)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);

    int a = 1;
    int b = 2;
    list_fpush(list, &a);
    list_fpush(list, &b);

    ASSERT_EQUAL(list_size(list), 2);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);

    int* value = list_fpopof(int, list);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 2);
    ASSERT_EQUAL(list_size(list), 1);

    value = list_fpopof(int, list);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 1);
    ASSERT_EQUAL(list_size(list), 0);

    value = list_fpopof(int, list);
    ASSERT_NULL(value);
    ASSERT_EQUAL(list_size(list), 0);

    list_free(list);
}

CTEST(list, list_bpop)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);

    int a = 1;
    int b = 2;
    list_fpush(list, &a);
    list_fpush(list, &b);

    ASSERT_EQUAL(list_size(list), 2);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);

    int* value = list_bpopof(int, list);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 1);
    ASSERT_EQUAL(list_size(list), 1);

    value = list_bpopof(int, list);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 2);
    ASSERT_EQUAL(list_size(list), 0);

    value = list_bpopof(int, list);
    ASSERT_NULL(value);
    ASSERT_EQUAL(list_size(list), 0);

    list_free(list);
}

CTEST(list, list_get)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);

    int a = 1;
    int b = 35;
    int c = -5;
    int d = 42;
    int e = 0;
    int f = 88;
    int g = 95;
    list_bpush(list, &a);
    list_bpush(list, &b);
    list_bpush(list, &c);
    list_bpush(list, &d);
    list_bpush(list, &e);
    list_bpush(list, &f);
    list_bpush(list, &g);

    ASSERT_EQUAL(list_size(list), 7);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);

    int* value = list_getof(int, list, 0);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 1);
    ASSERT_EQUAL(list_size(list), 7);

    value = list_getof(int, list, -1);
    ASSERT_NULL(value);
    ASSERT_EQUAL(list_size(list), 7);

    value = list_getof(int, list, 3);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 42);
    ASSERT_EQUAL(list_size(list), 7);

    value = list_getof(int, list, 6);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 95);
    ASSERT_EQUAL(list_size(list), 7);

    value = list_getof(int, list, 10);
    ASSERT_NULL(value);
    ASSERT_EQUAL(list_size(list), 7);

    list_free(list);
}

CTEST(list, list_remove)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);

    int a = 1;
    int b = 35;
    int c = -5;
    int d = 42;
    int e = 0;
    int f = 88;
    int g = 95;
    list_bpush(list, &a);
    list_bpush(list, &b);
    list_bpush(list, &c);
    list_bpush(list, &d);
    list_bpush(list, &e);
    list_bpush(list, &f);
    list_bpush(list, &g);

    ASSERT_EQUAL(list_size(list), 7);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);

    int* value = list_removeof(int, list, 2);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, -5);
    ASSERT_EQUAL(list_size(list), 6);

    value = list_removeof(int, list, 2);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 42);
    ASSERT_EQUAL(list_size(list), 5);

    value = list_removeof(int, list, -1);
    ASSERT_NULL(value);
    ASSERT_EQUAL(list_size(list), 5);

    value = list_removeof(int, list, 4);
    ASSERT_NOT_NULL(value);
    ASSERT_EQUAL(*value, 95);
    ASSERT_EQUAL(list_size(list), 4);

    value = list_removeof(int, list, 5);
    ASSERT_NULL(value);
    ASSERT_EQUAL(list_size(list), 4);

    list_free(list);
}

CTEST(list, list_insert)
{
    List* list = list_create();
    ASSERT_NOT_NULL(list);

    int a = 1;
    int b = 35;
    int c = -5;
    int d = 42;

    list_insert(list, &a, 0);
    ASSERT_EQUAL(list_size(list), 1);
    ASSERT_NOT_NULL(list->head);
    ASSERT_NOT_NULL(list->tail);

    list_insert(list, &b, 1);
    ASSERT_EQUAL(list_size(list), 2);
    int* value = list_getof(int, list, 1);
    ASSERT_EQUAL(*value, 35);

    list_insert(list, &d, 1);
    ASSERT_EQUAL(list_size(list), 3);
    value = list_getof(int, list, 2);
    ASSERT_EQUAL(*value, 35);
    value = list_getof(int, list, 1);
    ASSERT_EQUAL(*value, 42);

    list_insert(list, &c, 6);
    ASSERT_EQUAL(list_size(list), 3);
    value = list_getof(int, list, 2);
    ASSERT_EQUAL(*value, 35);
    value = list_getof(int, list, 1);
    ASSERT_EQUAL(*value, 42);

    list_free(list);
}