#ifndef LIST_H
#define LIST_H

#include "word.h"

typedef struct ll_node_t {
    word_t *word;
    struct ll_node_t *next;
} ll_node_t;

typedef struct linked_list {
    ll_node_t *head;
    ll_node_t *tail;
    int count;
} linked_list;

//Basic functions
void ll_init(linked_list *list);
void ll_insert_start(linked_list *list, word_t *word);
void ll_insert_end(linked_list *list, word_t *word);
void ll_insert_after(linked_list *list, ll_node_t *after, word_t *word);

//Higher level functions
void ll_count_word(linked_list *list, char *word);
ll_node_t *ll_find(linked_list *list, char *word);

//Cleanup
void ll_dispose(linked_list *list);

#endif
