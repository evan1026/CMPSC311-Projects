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

    if (after == list->tail) {
        list->tail = node;
    }

    list->count++;
}

void ll_count_word(linked_list *list, char *word) {
    ll_node_t *found_node = ll_find_place(list, word);
    word_t *word_to_insert;

    if (found_node == NULL) { // not found and belongs at front
        word_to_insert = make_word(word);
        ll_insert_start(list, word_to_insert);
    } else if (!word_matches(found_node->word, word)) { // not found, but comes after something
        word_to_insert = make_word(word);
        ll_insert_after(list, found_node, word_to_insert);
    } else { // found
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

ll_node_t *ll_find_place(linked_list *list, char *word) {
    ll_node_t *search = list->head;

    if (list->head == NULL || word_cmp(list->head->word, word) > 0) {
        return NULL;
    }

    while (search->next != NULL && word_cmp(search->next->word, word) <= 0) {
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
