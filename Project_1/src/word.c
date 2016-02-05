#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "word.h"

word_t *make_word(char *word_c) {
    word_t *out = (word_t *) malloc(sizeof(word_t));
    size_t string_length = strlen(word_c) + 1;
    char *new_word = (char *) malloc(sizeof(char) * string_length); // new string to allow for manipulation
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
