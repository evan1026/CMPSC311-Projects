/******************************************************************************
 * ~~~~~~~~~~                                                                 *
 * ~ list.c ~                                                                 *
 * ~~~~~~~~~~                                                                 *
 *                                                                            *
 * This file contains the code governing the function of the linked list      *
 * used in this project. This list contains nodes, each of which has a        *
 * string and a pointer to the next node. The list also keeps track of the    *
 * first and last nodes in the list, as well as how many nodes there are in   *
 * total.                                                                     *
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
 *         ll_insert_start(&list, word);                                      *
 *     }                                                                      *
 *     // do something with the data                                          *
 *     ll_dispose(&list);                                                     *
 *                                                                            *
 * Note: A number of these functions take in pointers but do not check to     *
 *       ensure they point to valid data. This was chosen because the only    *
 *       validation that can occur is to check if the pointers are NULL, and  *
 *       even then, the program ought to crash, as the programmer, rather     *
 *       than the user, used the interface incorrectly.                       *
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "list.h"

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
 * Inserts a word into the list at the very beginning                       *
 *                                                                          *
 * Arguments:                                                               *
 *     list (linked_list *) - the list to insert into                       *
 *     word (char *)        - the word to insert                            *
 *                                                                          *
 * Preconditions:                                                           *
 *     - both list and word point to valid data of their corresponding      *
 *       types that have been fully initialized                             *
 *                                                                          *
 * Postconditions:                                                          *
 *     - the word will be inserted at the start of the list, and the list's *
 *       count will be updated                                              *
 *                                                                          *
 * Warnings:                                                                *
 *     - a new node is allocated on the heap                                *
 ****************************************************************************/
void ll_insert_start(linked_list *list, const char *word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = strdup(word);
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
 * Inserts a word into the list at the very end                           *
 *                                                                        *
 * Arguments:                                                             *
 *     list (linked_list *) - the list to insert into                     *
 *     word (char *)        - the word to insert                          *
 *                                                                        *
 * Preconditions:                                                         *
 *     - both list and word point to valid data of their corresponding    *
 *       types that have been fully initialized                           *
 *                                                                        *
 * Postconditions:                                                        *
 *     - the word will be inserted at the end of the list, and the list's *
 *       count will be updated                                            *
 *                                                                        *
 * Warnings:                                                              *
 *     - a new node is allocated on the heap                              *
 **************************************************************************/
void ll_insert_end(linked_list *list, const char *word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = strdup(word);
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
 * Inserts a word into the list after a given node                            *
 *                                                                            *
 * Arguments:                                                                 *
 *     list  (linked_list *) - the list to insert into                        *
 *     after (ll_node_t *)   - the node to insert after                       *
 *     word  (char *)        - the word to insert                             *
 *                                                                            *
 * Preconditions:                                                             *
 *     - list, word, and after point to valid data of their corresponding     *
 *       types that have been fully initialized                               *
 *                                                                            *
 * Postconditions:                                                            *
 *     - the word will be inserted after the given node, and the list's count *
 *       will be updated                                                      *
 *                                                                            *
 * Warnings:                                                                  *
 *     - a new node is allocated on the heap                                  *
 ******************************************************************************/
void ll_insert_after(linked_list *list, ll_node_t *after, const char *word) {
    ll_node_t *node = (ll_node_t *) malloc(sizeof(ll_node_t));

    node->word = strdup(word);
    node->next = after->next;
    after->next = node;

    if (after == list->tail) {
        list->tail = node;
    }

    list->count++;
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
 *************************************************************************/
ll_node_t *ll_find(const linked_list *list, const char *word) {
    ll_node_t *search = list->head;

    while (search != NULL && strcmp(search->word, word) != 0) {
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
 *     - list is sorted in lexigraphical order                               *
 *                                                                           *
 * Warnings:                                                                 *
 *     - if the list is not sorted, the return value is essentially          *
 *       meaningless                                                         *
 *****************************************************************************/
ll_node_t *ll_find_place(const linked_list *list, const char *word) {
    ll_node_t *search = list->head;

    if (list->head == NULL || strcmp(list->head->word, word) > 0) {
        return NULL;
    }

    while (search->next != NULL && strcmp(search->next->word, word) <= 0) {
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
 *     - list is a valid list with all of its members on the heap (they will  *
 *       be as long as these functions are used to modify it)                 *
 *                                                                            *
 * Postconditions:                                                            *
 *     - all memory associated with the nodes of the list will be freed       *
 *                                                                            *
 * Warnings:                                                                  *
 *     - if the nodes were allocated on the stack rather than the heap, it    *
 *       could create issues, so only use these functions to deal with a      *
 *       linked list                                                          *
 *     - the list struct itself must still be freed if it was initially       *
 *       malloc'd                                                             *
 ******************************************************************************/
void ll_dispose(linked_list *list) {
    ll_node_t *temp;

    while (list->head != NULL) {
        free(list->head->word);

        temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}

/*****************************************************************************************
 * ~~~~~~~~~~~~~                                                                         *
 * ~ ll_sort() ~                                                                         *
 * ~~~~~~~~~~~~~                                                                         *
 *                                                                                       *
 * Sorts a linked list using a specialized version of merge sort                         *
 * This code is very heavily based on a linked list merge sort algorithm found online.   *
 * Original code at http://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.html *
 *                                                                                       *
 * Arguments:                                                                            *
 *     - list (linked_list *) - the list to sort                                         *
 *                                                                                       *
 * Postconditions:                                                                       *
 *     - The list, if it had any members at all, will have all of its members sorted     *
 *****************************************************************************************/
void ll_sort(linked_list *list) {
    ll_node_t *p, *q, *e;
    int insize, nmerges, psize, qsize, i;

    /*
     * Silly special case: if `list' was passed in as NULL, return
     * NULL immediately.
     */
    if (!list || !list->head)
        return;

    insize = 1;

    while (1) {
        p = list->head;
        list->head = NULL;
        list->tail = NULL;

        nmerges = 0;  /* count number of merges we do in this pass */

        while (p) {
            nmerges++;  /* there exists a merge to be done */
            /* step `insize' places along from p */
            q = p;
            psize = 0;
            for (i = 0; i < insize; i++) {
                psize++;

                q = q->next;
                if (!q) break;
            }

            /* if q hasn't fallen off end, we have two lists to merge */
            qsize = insize;

            /* now we have two lists; merge them */
            while (psize > 0 || (qsize > 0 && q)) {
                /* decide whether next element of merge comes from p or q */
                if (psize == 0) {
                    /* p is empty; e must come from q. */
                    e = q; q = q->next; qsize--;
                } else if (qsize == 0 || !q) {
                    /* q is empty; e must come from p. */
                    e = p; p = p->next; psize--;
                } else if (strcmp(p->word, q->word) <= 0) {
                    /* First element of p is lower (or same);
                     * e must come from p. */
                    e = p; p = p->next; psize--;
                } else {
                    /* First element of q is lower; e must come from q. */
                    e = q; q = q->next; qsize--;
                }

                /* add the next element to the merged list */
                if (list->tail) {
                    list->tail->next = e;
                } else {
                    list->head = e;
                }
                list->tail = e;
            }

            /* now p has stepped `insize' places along, and q has too */
            p = q;
        }

        list->tail->next = NULL;

        /* If we have done only one merge, we're finished. */
        if (nmerges <= 1)   /* allow for nmerges==0, the empty list case */
            return;

        /* Otherwise repeat, merging lists twice the size */
        insize *= 2;
    }
}
