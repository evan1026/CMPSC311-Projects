#ifndef WORD_H
#define WORD_H

#include <stdbool.h>
#include <string.h>

/************************************************
 * This file contains only basic documentation. *
 * For full code documentation, please see the  *
 * corresponding .c file                        *
 ************************************************/

// a structure that will hold a word and its count
typedef struct word_t {
    char *word;
    int count;
} word_t;

// function names are pretty intuitive
// for full docs, see word.c
word_t *make_word(char *word_c);
bool word_matches(const word_t *word, const char *word_c);
int word_cmp(const word_t *word, const char* word_c);
void word_dispose(word_t *word);

#endif
