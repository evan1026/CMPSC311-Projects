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

    return out;
}

bool word_matches(word_t *word, char *word_c) {
    int src_i = 0,
        dest_i = 0; // for iteration
    bool match = true;

    while (word_c[src_i] != 0) { // while not at end of string
        if (isalnum(word_c[src_i])) { // if char is alphanumeric
            if (word->word[dest_i] != tolower(word_c[src_i])) { // compare chars
                match = false;
                break;
            }
            ++dest_i;
        }
        ++src_i;
    }

    // at this point, we should be at the end of word
    // i.e., word->word[dest_i] == 0
    // so check that as well as the match

    return match && word->word[dest_i] == 0;
}

void word_dispose(word_t *word) {
    free(word->word);
    free(word);
}
