#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    FILE *input_textfile = fopen(argv[1, "r");
    FILE *output_countfile = fopen(argv[2], "w");
    FILE *output_runtime = fopen(argv[3], "w");

    if (check_files(FILE input_textfile, FILE output_countfile, FILE output_runtime){
        printf("All files opened succesfully");
    }






    if (close_files(FILE input_textfile, FILE output_countfile, FILE output_runtime){
        printf("All files closed succesfully");
    } 

    return 0;
}

//Checks if all the input and output files were opened succesfully
bool check_files(FILE input_textfile, FILE output_countfile, FILE output_runtime){

    bool opened_succesfully = true;

    if (input_textfile == NULL){
        fprintf(stderr, "Can't open input textfile\n");
        opened_succesfully = false;
    } 

    if (output_countfile == NULL){
        fprintf(stderr, "Can't open output countfile\n");
        opened_succesfully = false;
    }

    if (output_runtime == NULL){
        fprintf(stderr, "Can't open output runtime file\n");
        opened_succesfully = false;
    }

    return opened_succesfully;

}

//Closes all the files and returns whether they all closed successfully
bool close_files(FILE input_textfile, FILE output_countfile, FILE output_runtime){

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
        closed_succesfully = false;
    }

    return closed_succesfully;

}
