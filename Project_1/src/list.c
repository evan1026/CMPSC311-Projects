/******************************************************************************
 * ~~~~~~~~~~                                                                 *
 * ~ list.c ~                                                                 *
 * ~~~~~~~~~~                                                                 *
 *                                                                            *
 * This file contains the code governing the function of the linked list      *
 * used in this project. This list contains nodes, each of which has a        *
 * word object (which keeps track of the string and its count). The list      *
 * also keeps track of the first and last nodes in the list, as well as       *
 * how many nodes there are in total.                                         *
 *                                                                            *
 * It is up to the caller to create an object of type linked_list, but a      *
 * function is provided to initialize the list, namely ll_init(). This        *
 * function should be called before using any of the provided list functions, *
 * however the user can also just set the pointers to null and the count to   *
 * 0. The caller should also call ll_dispose() after they are done with the   *
 * list, as the nodes are allocated on the heap and need to be freed.         *
 *                                                                            *
 * Typical usage:                                                             *
 *     linked_list list;                                                      *
 *     ll_init(&list);                                                        *
 *     while (there are words to count) {                                     *
 *         ll_count_word(&list, word);                                        *
 *     }                                                                      *
 *     // do something with the data                                          *
 *     ll_dispose(&list);                                                     *
 *                                                                            *
 * It is also possible for the user to use the lower-level ll_insert_*()      *
 * functions, however, these will not keep the words in alphabetical order,   *
 * as per the assignment requirements.                                        *
 *                                                                            *
 * Note: A number of these functions take in pointers but do not check to     *
 *       ensure they point to valid data. This was chosen because the only    *
 *       validation that can occur is to check if the pointers are NULL, and  *
 *       even then, the program ought to crash, as the programmer, rather     *
 *       than the user, used the interface incorrectly.                       *
 ******************************************************************************/

#include <stdlib.h>

#include "list.h"
#include "word.h"

/*******************************************************************************
 * ~~~~~~~~~~~~~                                                               *
 * ~ ll_init() ~                                                               *
 * ~~~~~~~~~~~~~                                                               *                                                              *
 *                                                                             *
 * This function initializes a linked list object to it's initial, empty state *
 *                                                                             *
 * Arguments:                                                                  *
 *     list (linked_list *) - the list to initialize                           *
 *                                                                             *
 * Preconditions:                                                              *
 *     - list is a valid pointer to an uninitialized linked_list               *
 *                                                                             *
 * Postconditions:                                                             *
 *     - the linked_list pointed to by list will be properly initialized to be *
 *       used with the other functions in this file                            *
 *                                                                             *
 * Warnings:                                                                   *
 *     - if the list has already been initialized and has had elements added   *
 *       to it, unless there is a pointer to the first node all nodes will be  *
 *       lost, and the corresponding memory leaked.                            *
 *******************************************************************************/
void ll_init(linked_list *list) {
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

/****************************************************************************
 * ~~~~~~~~~~~~~~~~~~~~~                                                    *
 * ~ ll_insert_start() ~                                                    *
 * ~~~~~~~~~~~~~~~~~~~~~                                                    *
 *                                                                          *
 * Inserts a word_t into the list at the very beginning                     *
 *                                                                          *
 * Arguments:                                                               *
 *     list  (linked_list *) - the list to insert into                      *
 *     word (word_t *)       - the word to insert                           *
 *                                                                          *
 * Preconditions:                                                           *
 *     - both list and word point to valid structs of their corresponding   *
 *       types that have been fully initialized                             *
 *                                                                          *
 * Postconditions:                                                          *
 *     - the word will be inserted at the start of the list, and the list's *
 *       count will be updated                                              *
 *                                                                          *
 * Warnings:                                                                *
 *     - while this function does not modify word, ll_count_word() can      *
 *       modify it later since it becomes part of the list, so that is      *
 *       why it's not marked as const here                                  *
 ****************************************************************************/
void ll_insert_start(linked_list *list, word_t *word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = word;
    node->next = list->head;

    list->head = node;

    if (list->tail == NULL) {
        list->tail = node;
    }

    list->count++;
}

/**************************************************************************
 * ~~~~~~~~~~~~~~~~~~~                                                    *
 * ~ ll_insert_end() ~                                                    *
 * ~~~~~~~~~~~~~~~~~~~                                                    *
 *                                                                        *
 * Inserts a word_t into the list at the very end                         *
 *                                                                        *
 * Arguments:                                                             *
 *     list  (linked_list *) - the list to insert into                    *
 *     word (word_t *)       - the word to insert                         *
 *                                                                        *
 * Preconditions:                                                         *
 *     - both list and word point to valid structs of their corresponding *
 *       types that have been fully initialized                           *
 *                                                                        *
 * Postconditions:                                                        *
 *     - the word will be inserted at the end of the list, and the list's *
 *       count will be updated                                            *
 *                                                                        *
 * Warnings:                                                              *
 *     - while this function does not modify word, ll_count_word() can    *
 *       modify it later since it becomes part of the list, so that is    *
 *       why it's not marked as const here                                *
 **************************************************************************/
void ll_insert_end(linked_list *list, word_t *word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = word;
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


/******************************************************************************
 * ~~~~~~~~~~~~~~~~~~~~~                                                      *
 * ~ ll_insert_after() ~                                                      *
 * ~~~~~~~~~~~~~~~~~~~~~                                                      *
 *                                                                            *
 * Inserts a word_t into the list after a given node                          *
 *                                                                            *
 * Arguments:                                                                 *
 *     list  (linked_list *) - the list to insert into                        *
 *     after (ll_node_t *)   - the node to insert after                       *
 *     word  (word_t *)      - the word to insert                             *
 *                                                                            *
 * Preconditions:                                                             *
 *     - list, word, and after point to valid structs of their corresponding  *
 *       types that have been fully initialized                               *
 *                                                                            *
 * Postconditions:                                                            *
 *     - the word will be inserted after the given node, and the list's count *
 *       will be updated                                                      *
 *                                                                            *
 * Warnings:                                                                  *
 *     - while this function does not modify word, ll_count_word() can        *
 *       modify it later since it becomes part of the list, so that is        *
 *       why it's not marked as const here                                    *
 ******************************************************************************/
void ll_insert_after(linked_list *list, ll_node_t *after, word_t *word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = word;
    node->next = after->next;
    after->next = node;

    if (after == list->tail) {
        list->tail = node;
    }

    list->count++;
}

/******************************************************************************
 * ~~~~~~~~~~~~~~~~~~~                                                        *
 * ~ ll_count_word() ~                                                        *
 * ~~~~~~~~~~~~~~~~~~~                                                        *
 *                                                                            *
 * Uses the list to count a word, as per the assignment                       *
 * Using this function is preferable to the insertion methods above           *
 *                                                                            *
 * Arguments:                                                                 *
 *     list (linked_list *) - the list to use to count                        *
 *     word (char *)        - the word to count                               *
 *                                                                            *
 * Preconditions:                                                             *
 *     - list and word point to a valid list and string respectively          *
 *     - word points to a string that is a single word (not strictly needed   *
 *       for the function to run, however it's worth noting that the function *
 *       won't split words for you, although it will ignore non-alphanumeric  *
 *       characters)                                                          *
 *     - the words in list are in alphabetical order (i.e., this function was *
 *       the only one used to insert words to this list)                      *
 *                                                                            *
 * Postconditions:                                                            *
 *     - word will exist in the list and will have a count that is one more   *
 *       than before calling the function (assuming that not in the list      *
 *       counts as a count of 0)                                              *
 *     - word will be placed in the list based on it's alphabetical order     *
 *                                                                            *
 * Warnings:                                                                  *
 *     - if the list is not in alphabetical order, the word may be duplicated *
 *       in the list, so if you're going to use this function to insert, only *
 *       use this function                                                    *
 ******************************************************************************/
void ll_count_word(linked_list *list, char *word) {
    ll_node_t *found_node = ll_find_place(list, word);
    word_t *word_to_insert;

    if (found_node == NULL) { // not found and belongs at front
        word_to_insert = make_word(word);
        if (word_to_insert) ll_insert_start(list, word_to_insert);
    } else if (!word_matches(found_node->word, word)) { // not found, but comes after something
        word_to_insert = make_word(word);
        if (word_to_insert) ll_insert_after(list, found_node, word_to_insert);
    } else { // found
        found_node->word->count++;
    }
}

/*************************************************************************
 * ~~~~~~~~~~~~~                                                         *
 * ~ ll_find() ~                                                         *
 * ~~~~~~~~~~~~~                                                         *
 *                                                                       *
 * This function looks for a word in a list                              *
 *                                                                       *
 * Arguments:                                                            *
 *     list (linked_list *) - the list to search through                 *
 *     word (char *)        - the string to look for                     *
 *                                                                       *
 * Return:                                                               *
 *     - the ll_node_t corresponding to the found word if found, or NULL *
 *       otherwise                                                       *
 *                                                                       *
 * Preconditions:                                                        *
 *     - list and word are valid pointers to their respective types      *
 *                                                                       *
 * Postconditions:                                                       *
 *     - none                                                            *
 *                                                                       *
 * Warnings:                                                             *
 *     - none                                                            *
 *************************************************************************/
ll_node_t *ll_find(const linked_list *list, const char *word) {
    ll_node_t *search = list->head;

    while (search != NULL && !word_matches(search->word, word)) {
        search = search->next;
    }

    return search;
}

/*****************************************************************************
 * ~~~~~~~~~~~~~~~~~~~                                                       *
 * ~ ll_find_place() ~                                                       *
 * ~~~~~~~~~~~~~~~~~~~                                                       *
 *                                                                           *
 * Finds where a string should be inserted in a sorted list                  *
 *                                                                           *
 * Arguments:                                                                *
 *     list (linked_list *) - list list to search through                    *
 *     word (char *)        - the string to look for                         *
 *                                                                           *
 * Return:                                                                   *
 *     - the ll_node_t containing the word if it is found, or the node it    *
 *       should come after if not found, or NULL if it should be placed at   *
 *       the start                                                           *
 *                                                                           *
 * Preconditions:                                                            *
 *     - list and word are valid pointers to their respective types          *
 *     - list is sorted in lexigraphical order (i.e., ll_count_word() is all *
 *       that was used to insert values)                                     *
 *                                                                           *
 * Postconditions:                                                           *
 *     - none                                                                *
 *                                                                           *
 * Warnings:                                                                 *
 *     - if the list is not sorted, the return value is essentially          *
 *       meaningless                                                         *
 *****************************************************************************/
ll_node_t *ll_find_place(const linked_list *list, const char *word) {
    ll_node_t *search = list->head;

    if (list->head == NULL || word_cmp_c(list->head->word, word) > 0) {
        return NULL;
    }

    while (search->next != NULL && word_cmp_c(search->next->word, word) <= 0) {
        search = search->next;
    }

    return search;
}

/******************************************************************************
 * ~~~~~~~~~~~~~~~~                                                           *
 * ~ ll_dispose() ~                                                           *
 * ~~~~~~~~~~~~~~~~                                                           *
 *                                                                            *
 * Does cleanup on the list by freeing the memory associated with each of the *
 * members                                                                    *
 *                                                                            *
 * Arguments:                                                                 *
 *     - list (linked_list *) - the list to clean up                          *
 *                                                                            *
 * Preconditions:                                                             *
 *     - list is a valid list with all of its members on the heap (as they    *
 *       will be if these ll_count_word() is used to add members)             *
 *                                                                            *
 * Postconditions:                                                            *
 *     - all memory associated with the nodes of the list will be freed       *
 *                                                                            *
 * Warnings:                                                                  *
 *     - if the nodes were allocated on the stack rather than the heap, it    *
 *       could create issues, so please only use ll_count_word() unless you   *
 *       know what you're doing                                               *
 *     - the list struct itself must still be freed if it was initially       *
 *       malloc'd                                                             *
 ******************************************************************************/
void ll_dispose(linked_list *list) {
    ll_node_t *temp;

    while (list->head != NULL) {
        word_dispose(list->head->word);

        temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}
