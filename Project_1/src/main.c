#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

bool check_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime);
bool close_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime);

int main(int argc, char *argv[]) {

    FILE *input_textfile = fopen(argv[1], "r");
    FILE *output_countfile = fopen(argv[2], "w");
    FILE *output_runtime = fopen(argv[3], "w");
    char input_word[1024];
    linked_list words_list;
    ll_node_t *curr = NULL;

    if (check_files(input_textfile, output_countfile, output_runtime)){
        printf("All files opened successfully\n");
    } else {
        return 1;
    }

    ll_init(&words_list);

    while (!feof(input_textfile)){
        fscanf(input_textfile, "%s", input_word);
        ll_count_word(&words_list, input_word);
    }

    curr = words_list.head;

    while (curr != NULL) {
        if (fprintf(output_countfile, "%s, %d\n", curr->word->word, curr->word->count) == EOF){
            fprintf(stderr, "Error printing to countfile");
        }
        curr = curr->next;
    }


    if (close_files(input_textfile, output_countfile, output_runtime)){
        printf("All files closed successfully\n");
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
