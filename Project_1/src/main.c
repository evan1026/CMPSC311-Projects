#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "list.h"

bool check_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime);
bool close_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime);

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

    //checks for erraneous input
    if (argv[1] != NULL && argv[2] != NULL && argv[3] != NULL && argv[4] == NULL){
        input_textfile = fopen(argv[1], "r");
        output_countfile = fopen(argv[2], "w");
        output_runtime = fopen(argv[3], "w");
    } else{
        printf("Erraneous input supplied\n");
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
        fscanf(input_textfile, "%s", input_word);
        ll_count_word(&words_list, input_word);
    }

    curr = words_list.head;

    //prints each word and its count to the output file and checks for errors each time
    while (curr != NULL) {
        if (fprintf(output_countfile, "%s, %d\n", curr->word->word, curr->word->count) == EOF){
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

    //checks to make sure all files were closed succesfully
    if (close_files(input_textfile, output_countfile, output_runtime)){
        printf("All files closed successfully\n");
    } else{
        return 1;
    }
    
    return 0;
}

//Checks if all the input and output files were opened successfully
bool check_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime){

    bool opened_successfully = true;

    if (input_textfile == NULL){
        fprintf(stderr, "Can't open input textfile\n");
        opened_successfully = false;
    }

    if (output_countfile == NULL){
        fprintf(stderr, "Can't open output countfile\n");
        opened_successfully = false;
    }

    if (output_runtime == NULL){
        fprintf(stderr, "Can't open output runtime file\n");
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
