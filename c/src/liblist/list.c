#include <liblist/list.h>

List* list_create()
{
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL)
        return NULL;

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

ListNode* listnode_create(void* value)
{
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    if (node == NULL)
        return NULL;

    node->previous = NULL;
    node->next = NULL;
    node->value = value;
    return node;
}

void listnode_free(ListNode* node)
{
    if (node == NULL)
        return;
    node->previous = NULL;
    node->next = NULL;
    node->value = NULL;
    free(node);
}

ListNode* listnode_find(List* list, unsigned int index)
{
    ListNode* current = list->head;
    unsigned int i = 0;
    while (current != NULL) {
        if (i == index)
            break;
        current = current->next;
        i++;
    }
    return current;
}

void list_clear(List* list)
{
    if (list == NULL)
        return;

    ListNode* curr_node = list->head;
    ListNode* next_node = NULL;
    while (curr_node != NULL) {
        next_node = curr_node->next;
        listnode_free(curr_node);
        curr_node = next_node;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_fpush(List* list, void* value)
{
    if (list == NULL)
        return;
    ListNode* new_node = listnode_create(value);
    if (new_node == NULL)
        return;

    if (list->head != NULL)
        list->head->previous = new_node;

    new_node->next = list->head;
    list->head = new_node;

    if (list_is_empty(list))
        list->tail = new_node;

    list->size++;
}

void* list_fpop(List* list)
{
    if (list == NULL)
        return NULL;
    ListNode* node = list->head;
    if (node == NULL)
        return NULL;

    list->size--;

    void* value = node->value;
    if (list_is_empty(list)) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        node->next->previous = NULL;
        list->head = node->next;
    }
    listnode_free(node);

    return value;
}

void list_bpush(List* list, void* value)
{
    if (list == NULL)
        return;
    ListNode* new_node = listnode_create(value);
    if (new_node == NULL)
        return;

    if (list->tail != NULL)
        list->tail->next = new_node;

    new_node->previous = list->tail;
    list->tail = new_node;

    if (list_is_empty(list))
        list->head = new_node;

    list->size++;
}

void* list_bpop(List* list)
{
    if (list == NULL)
        return NULL;
    ListNode* node = list->tail;
    if (node == NULL)
        return NULL;

    list->size--;

    void* value = node->value;
    if (list_is_empty(list)) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        node->previous->next = NULL;
        list->tail = node->previous;
    }
    listnode_free(node);

    return value;
}

void* list_get(List* list, unsigned int index)
{
    if (list == NULL)
        return NULL;
    if (0 >= list_size(list))
        return NULL;

    ListNode* current = listnode_find(list, index);

    if (current == NULL)
        return NULL;
    else
        return current->value;
}

void* list_first(List* list)
{
    if (list == NULL)
        return NULL;

    if (list->head != NULL)
        return list->head->value;
    else
        return NULL;
}

void* list_last(List* list)
{
    if (list == NULL)
        return NULL;

    if (list->tail != NULL)
        return list->tail->value;
    else
        return NULL;
}

void* list_remove(List* list, unsigned int index)
{
    if (list == NULL)
        return NULL;

    if (index >= list_size(list))
        return NULL;

    ListNode* current = listnode_find(list, index);

    void* value = current->value;

    if (current->previous != NULL) // is not head?
        current->previous->next = current->next;
    else // is head?
        list->head = current->next;

    if (current->next != NULL) // is not tail?
        current->next->previous = current->previous;
    else // is tail?
        list->tail = current->previous;

    list->size--;
    listnode_free(current);

    return value;
}

void list_insert(List* list, void* value, unsigned int index)
{
    if (list == NULL)
        return;

    if (index > list_size(list))
        return;

    if (index == 0)
        return list_fpush(list, value);
    else if (index == list_size(list))
        return list_bpush(list, value);

    ListNode* newNode = listnode_create(value);

    ListNode* next = listnode_find(list, index);
    ListNode* previous = next->previous;

    previous->next = newNode;
    newNode->previous = previous;

    next->previous = newNode;
    newNode->next = next;

    list->size++;
}

List* list_copy(const List* from, List* to)
{
    if (from == NULL || to == NULL)
        return NULL;

    if (list_is_empty(from))
        return to;

    const ListNode* fnode = from->head;
    ListNode *last_tnode = NULL, *tnode = listnode_create(fnode->value);

    if (tnode == NULL)
        return to;

    list_clear(to);

    to->head = tnode;
    to->size = list_size(from);

    while ((fnode = fnode->next) != NULL) {
        last_tnode = tnode;
        tnode = listnode_create(fnode->value);
        if (tnode == NULL) {
            list_clear(to);
            return to;
        }
        last_tnode->next = tnode;
        tnode->previous = last_tnode;
    }

    to->tail = tnode;
    return to;
}