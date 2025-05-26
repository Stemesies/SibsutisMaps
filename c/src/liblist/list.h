#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

typedef struct LIST_NODE {
    struct LIST_NODE* previous;
    struct LIST_NODE* next;
    void* value;
} ListNode;

typedef struct LIST {
    unsigned int size;
    ListNode* head;
    ListNode* tail;
} List;

#define list_size(list) list->size
#define list_is_empty(list) list->size == 0
#define list_itp(type) (((type*)it))
#define list_it(type) (*((type*)it))

#define list_fpopof(type, list) ((type*)list_fpop(list))
#define list_bpopof(type, list) ((type*)list_bpop(list))
#define list_removeof(type, list, index) ((type*)list_remove(list, index))
#define list_getof(type, list, index) ((type*)list_get(list, index))
#define list_firstof(type, list) ((type*)list_first(list))
#define list_lastof(type, list) ((type*)list_last(list))

#define listNewCopy(from) list_copy((from), list_create())

#define listForEachInlined(list, bbbblock)  \
    if (1) {                                \
        ListNode* node = list->head;        \
        void* it = NULL;                    \
        while (node != NULL) {              \
            it = node->value;               \
            if (1)                          \
                bbbblock node = node->next; \
        }                                   \
    }

/*
Очищает и освобождает данный лист.
Имейте в виду, что этот метод не освобождает значения, хранящиеся в листе.
*/
#define list_free(list) \
    list_clear(list);   \
    free(list);

List* list_create();
void list_clear(List* list);
void list_fpush(List* list, void* value);
void* list_fpop(List* list);
void list_bpush(List* list, void* value);
void* list_bpop(List* list);
void* list_get(List* list, unsigned int index);
void* list_first(List* list);
void* list_last(List* list);
void* list_remove(List* list, unsigned int index);
void list_insert(List* list, void* value, unsigned int index);
List* list_copy(const List* from, List* to);

#endif