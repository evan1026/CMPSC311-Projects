#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "list.h"
#include "hashtable.h"

/**********************************************************************
 * One question you might ask after reading this and the makefile is, *
 * "Why put so much effort into clock_gettime()?"                     *
 *                                                                    *
 * Well, it turns out that gettimeofday() has a maximum resolution of *
 * microseconds and, more importantly, has no guarantee that the time *
 * will always increase, because it uses system time, which can be    *
 * changed by a system administrator. clock_gettime(), however, uses  *
 * the actual hardware time, so it will always increase between       *
 * subsequent calls.                                                  *
 **********************************************************************/

bool check_files(const FILE *input_textfile, const FILE *output_countfile, const FILE *output_runtime);
bool close_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime);
char *clean_word(const char *word);

int main(int argc, char *argv[]) {

    FILE *input_textfile = NULL;
    FILE *output_countfile = NULL;
    FILE *output_runtime = NULL;
    char input_word[1024];
    linked_list words_list;
    ll_node_t *curr = NULL;
    struct timespec start_time;
    struct timespec end_time;
    struct timespec time_elapsed;
    hashtable_t *hashtable = ht_create(65536);

    //checks for erroneous input
    if (argc == 4){
        input_textfile = fopen(argv[1], "r");
        output_countfile = fopen(argv[2], "w");
        output_runtime = fopen(argv[3], "w");
    } else{
        printf("Erroneous input supplied\n");
        printf("The program should be run with ./wordc input_textfile output_countfile output_runtime\n");
        return 1;
    }

    //checks to make sure start time is correctly obtained
    if (clock_gettime(CLOCK_REALTIME, &start_time) == -1){
        fprintf(stderr, "Error getting starting clocktime");
    }

    //checks to make sure all files are opened correctly
    if (check_files(input_textfile, output_countfile, output_runtime)){
        printf("All files opened successfully\n");
    } else {
        return 1;
    }

    ll_init(&words_list);

    //read all words from the file and add them to the linked list
    while (!feof(input_textfile)){
        char *temp_word;

        fscanf(input_textfile, "%s", input_word);

        temp_word = clean_word(input_word);
        if (temp_word != NULL){
            int result = ht_get(hashtable, temp_word);
            if (result == -1) {
                ht_set(hashtable, temp_word, 1);
                ll_insert_start(&words_list, temp_word);
            } else {
                ht_set(hashtable, temp_word, result + 1);
            }
            free(temp_word);
        }
    }

    ll_sort(&words_list);
    curr = words_list.head;

    //prints each word and its count to the output file and checks for errors each time
    while (curr != NULL) {
        if (fprintf(output_countfile, "%s, %d\n", curr->word, ht_get(hashtable, curr->word)) == EOF){
            fprintf(stderr, "Error printing to countfile");
        }
        curr = curr->next;
    }

    //chechs to make sure the end time is correctly obtained
    if (clock_gettime(CLOCK_REALTIME, &end_time) == -1){
        fprintf(stderr, "Error getting ending clock time");
    }

    //calculates time elapsed
    time_elapsed.tv_nsec = (end_time.tv_sec * 1000000000 + end_time.tv_nsec) - (start_time.tv_sec * 1000000000 + start_time.tv_nsec);

    //outputs time elapsed to files and checks if it's outputted correctly
    if (fprintf(output_runtime, "The number of nanoseconds elapsed is %ld\n", time_elapsed.tv_nsec) == EOF){
        fprintf(stderr, "Error printing to runtime output file");
    }

    ll_dispose(&words_list);
    ht_dispose(hashtable);

    //checks to make sure all files were closed succesfully
    if (close_files(input_textfile, output_countfile, output_runtime)){
        printf("All files closed successfully\n");
    } else{
        return 1;
    }

    return 0;
}

//Checks if all the input and output files were opened successfully
bool check_files(const FILE *input_textfile, const FILE *output_countfile, const FILE *output_runtime){

    bool opened_successfully = true;

    if (input_textfile == NULL){
        fprintf(stderr, "Can't open input textfile. Please make sure the file exists and you have read permissions for it.\n");
        opened_successfully = false;
    }

    if (output_countfile == NULL){
        fprintf(stderr, "Can't open output countfile. Please make sure you have write permissions to that location.\n");
        opened_successfully = false;
    }

    if (output_runtime == NULL){
        fprintf(stderr, "Can't open output runtime file. Please make sure you have write permissions to that location.\n");
        opened_successfully = false;
    }

    return opened_successfully;

}

//Closes all the files and returns whether they all closed successfully
bool close_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime){

    bool closed_successfully = true;

    if (fclose(input_textfile) == EOF){
        fprintf(stderr, "Error closing input textfile\n");
        closed_successfully = false;
    }

    if (fclose(output_countfile) == EOF){
        fprintf(stderr, "Error closing output countfile\n");
        closed_successfully = false;
    }

    if (fclose(output_runtime) == EOF){
        fprintf(stderr, "Error closing output runtime\n");
        closed_successfully = false;
    }

    return closed_successfully;

}

// removes all symbols from a word and puts it to lowercase, then
// returns the new word
char *clean_word(const char *word) {
    int length = strlen(word) + 1;
    char *out = malloc(sizeof(char) * length);
    if (out == NULL) return NULL;
    int src_i = 0,
        dest_i = 0;

    while (word[src_i] != 0) {
        if (isalnum(word[src_i])) { // if char is alphanumeric
            out[dest_i] = tolower(word[src_i]); // set char to lowercase and copy
            ++dest_i;
        }
        ++src_i;
    }

    if (dest_i == 0) { // If it's empty, return null
        free(out);
        out = NULL;
    } else {
        out[dest_i] = 0; // add null char
    }

    return out;
}
