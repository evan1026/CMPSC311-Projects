#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    FILE *input_textfile = fopen(argv[1, "r");
    FILE *output_countfile = fopen(argv[2], "w");
    FILE *output_runtime = fopen(argv[3], "w");

    if (check_files(FILE input_textfile, FILE output_countfile, FILE output_runtime){
        printf("All files opened succesfully");
    }

    

}

bool check_files(FILE input_textfile, FILE output_countfile, FILE output_runtime){

    bool opened_succesfully = true;

    if (input_textfile == NULL){
        fprintf(stderr, "Can't open input textfile");
        opened_succesfully = false;
    } 

    if (output_countfile == NULL){
        fprintf(stderr, "Can't open output countfile");
        opened_succesfully = false;
    }

    if (output_runtime == NULL){
        fprintf(stderr, "Can't open output runtime file");
        opened_succesfully = false;
    }

    return opened_succesfully;

}
