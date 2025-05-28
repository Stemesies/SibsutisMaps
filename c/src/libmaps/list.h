#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

typedef struct listnode {
    struct listnode* previous;
    struct listnode* next;
    void* value;
} ListNode;

typedef struct list {
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

#define list_foreach_inlined(list, bbbblock) \
    if (1) {                                 \
        ListNode* node = list->head;         \
        void* it = NULL;                     \
        while (node != NULL) {               \
            it = node->value;                \
            if (1)                           \
                bbbblock node = node->next;  \
        }                                    \
    }

/*
Очищает и освобождает данный лист.
Имейте в виду, что этот метод не освобождает значения, хранящиеся в листе.
*/
#define list_free(list) \
    list_clear(list);   \
    free(list);

/*
Выделяет память под новый связный список
*/
List* list_create();

/*
Очищает заданный список.
Учтите: данный метод не уничтожает значения, хранящиеся в листе.
*/
void list_clear(List* list);

/*
Добавляет элемент в начало списка.
*/
void list_fpush(List* list, void* value);

/*
Убирает элемент из начала списка.
Возвращает: значение, хранящееся в начальном ноде.
Null - если лист был пустой.
*/
void* list_fpop(List* list);

/*
Добавляет элемент в конец списка.
*/
void list_bpush(List* list, void* value);

/*
Убирает элемент из конца списка.
Возвращает: значение, хранящееся в конечном ноде.
Null - если лист был пустой.
*/
void* list_bpop(List* list);

/*
Данный метод возвращает значение, хранящееся по указанному индексу.
Если индекс за пределами листа или если лист пустой - возвращает NULL.
*/
void* list_get(List* list, unsigned int index);

/*
Возвращает: значение, хранящееся в начальном ноде.
Null - если лист пустой.
*/
void* list_first(List* list);

/*
Возвращает: значение, хранящееся в конечном ноде.
Null - если лист пустой.
*/
void* list_last(List* list);

/*
Убирает элемент, находящийся на указанной позиции.
Возвращает: значение убранного элемента.
Если индекс за пределами листа или если лист пустой - возвращает NULL.
*/
void* list_remove(List* list, unsigned int index);

/*
Вставляет элемент в указанную позицию.
Если индекс за пределами листа - ничего не происходит.
*/
void list_insert(List* list, void* value, unsigned int index);

/*
Очищает лист to и копирует туда все значения, находящиеся в from.
*/
List* list_copy(const List* from, List* to);

#endif
