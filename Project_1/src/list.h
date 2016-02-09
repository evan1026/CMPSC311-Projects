#ifndef LIST_H
#define LIST_H

/************************************************
 * This file contains only basic documentation. *
 * For full code documentation, please see the  *
 * corresponding .c file                        *
 ************************************************/

// Type representing one node within the linked list
typedef struct ll_node_t {
    char *word;
    struct ll_node_t *next;
} ll_node_t;

// The linked list itself, which need only keep track of the first and
// last nodes, and I've included the count because it could be useful
typedef struct linked_list {
    ll_node_t *head;
    ll_node_t *tail;
    int count;
} linked_list;

// Basic functions - full docs on each function in word.c
void ll_init(linked_list *list);
void ll_insert_start(linked_list *list, const char *word);
void ll_insert_end(linked_list *list, const char *word);
void ll_insert_after(linked_list *list, ll_node_t *after, const char *word);
void ll_dispose(linked_list *list);

// Higher level functions - fulls docs on each function in word.c
ll_node_t *ll_find(const linked_list *list, const char *word);
ll_node_t *ll_find_place(const linked_list *list, const char *word);
void ll_sort(linked_list *list);
#endif
