#ifndef WORD_H
#define WORD_H

#include <stdbool.h>
#include <string.h>

typedef struct word_t {
    char *word;
    int count;
} word_t;

word_t *make_word(char *word_c);
bool word_matches(word_t *word, char *word_c);
int word_cmp(word_t *word, char* word_c);
void word_dispose(word_t *word);

#endif
