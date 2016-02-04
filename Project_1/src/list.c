#include <stdlib.h>

#include "list.h"
#include "word.h"

void ll_init(linked_list *list) {
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

void ll_insert_start(linked_list *list, word_t *_word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = _word;
    node->next = list->head;

    list->head = node;

    if (list->tail == NULL) {
        list->tail = node;
    }

    list->count++;
}

void ll_insert_end(linked_list *list, word_t *_word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = _word;
    node->next = NULL;

    if (list->tail != NULL) {
        list->tail->next = node;
    }

    list->tail = node;

    if (list->head == NULL) {
        list->head = node;
    }

    list->count++;
}

// shouldn't be needed, but nice to have
void ll_insert_after(linked_list *list, ll_node_t *after, word_t *_word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = _word;
    node->next = after->next;
    after->next = node;

    list->count++;
}

void ll_count_word(linked_list *list, char *word) {
    ll_node_t *found_node = ll_find(list, word);
    word_t *word_to_insert;

    if (found_node == NULL) { // not found - add it
        word_to_insert = make_word(word); // starts count at 1 - no need to modify
        ll_insert_end(list, word_to_insert);
    } else {
        found_node->word->count++;
    }
}

ll_node_t *ll_find(linked_list *list, char *word) {
    ll_node_t *search = list->head;

    while (search != NULL && !word_matches(search->word, word)) {
        search = search->next;
    }

    return search;
}

void ll_dispose(linked_list *list) {
    ll_node_t *temp;

    while (list->head != NULL) {
        word_dispose(list->head->word);

        temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}
