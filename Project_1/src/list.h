#ifndef LIST_H
#define LIST_H

#include "word.h"

/************************************************
 * This file contains only basic documentation. *
 * For full code documentation, please see the  *
 * corresponding .c file                        *
 ************************************************/

// Type representing one node within the linked list
typedef struct ll_node_t {
    word_t *word;
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
void ll_insert_start(linked_list *list, word_t *word);
void ll_insert_end(linked_list *list, word_t *word);
void ll_insert_after(linked_list *list, ll_node_t *after, word_t *word);
void ll_dispose(linked_list *list);

// Higher level functions - fulls docs on each function in word.c
void ll_count_word(linked_list *list, char *word);
ll_node_t *ll_find(const linked_list *list, const char *word);
ll_node_t *ll_find_place(const linked_list *list, const char *word);

#endif
