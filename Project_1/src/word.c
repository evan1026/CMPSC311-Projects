/********************************************************************************
 * ~~~~~~~~~~                                                                   *
 * ~ word.c ~                                                                   *
 * ~~~~~~~~~~                                                                   *
 *                                                                              *
 * This file contains code dealing with the word_t struct, which holds a word   *
 * and its count. A utility function called make_word() is provided that will   *
 * automatically create a word_t from a string, and it will strip out any non-  *
 * alphanumeric characters. This function allocates the object on the heap, so  *
 * it is important that the user call word_dispose() later on if they choose to *
 * use this function. This function will free both the word itself as well as   *
 * the string contained within.                                                 *
 *                                                                              *
 * Typical usage:                                                               *
 *     word_t *word = make_word("cake");                                        *
 *     // do stuff                                                              *
 *     word_dispose(word);                                                      *
 ********************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "word.h"

/*
 * ~~~
 * ~ make_word() ~
 * ~~~
 *
 * This function creates a word_t object (on the heap) representing the string
 * given as its parameter. It will automatically remove non-alphanumeric
 * characters and change all letters to lowercase. If, after these alterations,
 * the string is empty, it will return NULL.
 *
 *
 */
word_t *make_word(char *word_c) {
    word_t *out  = (word_t *) malloc(sizeof(word_t));
    char *new_word = (char *) malloc(sizeof(char) * (strlen(word_c) + 1)); // new string to allow for manipulation
    int src_i = 0,
        dest_i = 0; // for iteration

    while (word_c[src_i] != 0) { // while not at end of string
        if (isalnum(word_c[src_i])) { // if char is alphanumeric
            new_word[dest_i] = tolower(word_c[src_i]); // set char to lowercase and copy
            ++dest_i;
        }
        ++src_i;
    }
    new_word[dest_i] = 0; // insert null char

    out->word = new_word;
    out->count = 1; // If we're making the word, it's safe to assume we've found it, and it's the first time

    if (dest_i == 0) {
        word_dispose(out);
        out = NULL;
    }

    return out;
}

bool word_matches(word_t *word, char *word_c) {
    return word_cmp(word, word_c) == 0;
}

//works like strcmp
int word_cmp(word_t *word, char *word_c) {
    int src_i = 0,
        dest_i = 0, // for iteration
        diff; // for comparison

    while (word_c[src_i] != 0 && word->word[dest_i] != 0) { // while not at end of string
        if (isalnum(word_c[src_i])) { // if char is alphanumeric
            diff = word->word[dest_i] - tolower(word_c[src_i]);
            if (diff != 0) { // compare chars
                return diff;
            }
            ++dest_i;
        }
        ++src_i;
    }

    // now we're at the end of at least one word
    // however, word_c may have trailing non-alphanumeric characters
    // So let's skip past those
    while (word_c[src_i] != 0 && !isalnum(word_c[src_i])) {
        ++src_i;
    }

    // at this point, if they're the same, we're at the end of both
    // otherwise, we're at the end of one but not the other
    // returning the same diff should do it

    return word->word[dest_i] - tolower(word_c[src_i]);
}

void word_dispose(word_t *word) {
    free(word->word);
    free(word);
}
