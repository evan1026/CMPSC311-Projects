#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "list.h"
#include "hashtable.h"
#include "elapsedtime.h"

void handle_input(int argc, char *argv[], FILE **input_textfile, FILE **output_countfile, FILE **output_runtime, int *num_processes);
bool check_files(const FILE *input_textfile, const FILE *output_countfile, const FILE *output_runtime);
bool close_files(FILE *input_textfile, FILE *output_countfile, FILE *output_runtime);
char *clean_word(const char *word);
void count_word(const char* word, hashtable_t *hashtable, linked_list *words_list, int occurances);

int main(int argc, char *argv[]) {

    FILE *input_textfile = NULL;
    FILE *output_countfile = NULL;
    FILE *output_runtime = NULL;
    FILE **child_pipes = NULL;
    FILE *parent_pipe = NULL;
    char input_word[1024];
    linked_list words_list;
    ll_node_t *curr = NULL;
    hashtable_t *hashtable = ht_create(65536);
    int num_processes = 0;
    int proc_num = 0; //The index of this process (used after forking)
    int fd_pipe[2];
    long file_size = -1;
    char c;

    //checks for erroneous input and opens files
    handle_input(argc, argv, &input_textfile, &output_countfile, &output_runtime, &num_processes);

    start_clock();

    //checks to make sure all files are opened correctly
    if (check_files(input_textfile, output_countfile, output_runtime)){
        printf("All files opened successfully\n");
    } else {
        return 1;
    }

    ll_init(&words_list);

    if (num_processes > 1) {
        child_pipes = malloc(num_processes * sizeof(FILE *));

        if (child_pipes == NULL) {
            perror("Could not reserve space for pipes.");
            return 1;
        }

        for (proc_num = 0; proc_num < num_processes - 1; proc_num++) {
            if (pipe(fd_pipe) == -1){
                fprintf(stderr, "Could not create pipe for process index %d", proc_num);
                perror("");
                return 1;
            }

            pid_t proc = fork();

            if (proc == 0) {
                parent_pipe = fdopen(fd_pipe[1], "w");
                close(fd_pipe[0]); //close unused read end
                break;
            }
            else if (proc == -1) {
                fprintf(stderr,"Could not fork process index %d", proc_num);
                perror("");
                return 1;
            }

            //if we get here, we are parent
            child_pipes[proc_num] = fdopen(fd_pipe[0], "r");
            close(fd_pipe[1]); //close unused write end
        }
    }


    input_textfile = fopen(argv[1], "rb");

    //This code taken from http://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    //Interestingly, it also fixes a bug we had where the child would start reading at an unpredictable place
    //No idea why, but apparently the offset wasn't guarenteed to start at 0 for some reason
    fseek(input_textfile, 0L, SEEK_END);
    file_size = ftell(input_textfile);
    fseek(input_textfile, proc_num * (1.0 * file_size / num_processes), 0);

    //read all words from the file and add them to the linked list
    if (file_size != 0) {
        //If we're not at the start of the file (proc_num != 0) then we're probably
        //in the middle of a word. Let's advance past it.
        if (proc_num != 0) {
            while (!isspace(c = fgetc(input_textfile)));
            ungetc(c, input_textfile); //Put that non-whitespace char back, because we just removed it
        }

        //Explaination of this mess of a while loop:
        //  if we're a child process (proc_num < num_processes - 1), then loop until we make it to where the next
        //  process would start (the ftell part)
        //  if we're the parent (proc_num == num_processes - 1), loop until we reach the end of the file
        //Going to the end originally used feof, but for some reason, that would cause the children to sometimes
        //(but not always and not even predictably) reach EOF before they've gotten through their portion
        //Doing it this way fixes this error
        while ((proc_num < num_processes - 1 && ftell(input_textfile) < (proc_num + 1) * (1.0 * file_size / num_processes))
                || (proc_num == num_processes - 1 && ftell(input_textfile) < file_size)){
            int res = fscanf(input_textfile, "%s", input_word);

            if (res == 1) {
                count_word(input_word, hashtable, &words_list, 1);
            } else if (res == EOF && errno != 0) {
                perror("Error reading file: ");
                exit(1);
            } else if (res == EOF && ftell(input_textfile) < file_size) {
                printf("Process %d found unexpected EOF at %ld.\n", proc_num, ftell(input_textfile));
                exit(1);
            } else if (res == EOF && feof(input_textfile)){
                continue;
            } else {
                printf("Scanf returned unexpected value: %d\n", res);
                exit(1);
            }
        }
    }

    if (proc_num < num_processes - 1) { //then we are a child
        curr = words_list.head;
        while (curr != NULL) {
            fprintf(parent_pipe, "%s %d\n", curr->word, ht_get(hashtable, curr->word));
            curr = curr->next;
        }
        fclose(parent_pipe);
    } else {
        char buffer[1024];
        int count = -1;
        while (waitpid(-1, NULL, WNOHANG) != -1) { //While not all children have exited
            //go through and read what they've written to pipes, if anything
            for (int i = 0; i < num_processes - 1; i++) {
                while (fgets(buffer, 1024, child_pipes[i])) {
                    //Scanf string from http://stackoverflow.com/questions/12835360/whitespace-in-the-format-string-scanf
                    sscanf(buffer, "%s%*[ ]%d%*[\n]", input_word, &count);
 //                   printf("Parent: Child %d says: %s is %d\n", i, input_word, count);
                    count_word(input_word, hashtable, &words_list, count);
                }
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

        end_clock();

        //outputs time elapsed to files and checks if it's outputted correctly
        if (fprintf(output_runtime, "The number of nanoseconds elapsed is %ld\n", get_time_elapsed()) == EOF){
            fprintf(stderr, "Error printing to runtime output file");
        }
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

void count_word(const char* word, hashtable_t *hashtable, linked_list *words_list, int occurances) {
    char *temp_word = clean_word(word);

    if (temp_word != NULL){
        int result = ht_get(hashtable, temp_word);
        if (result == 0) {
            ll_insert_start(words_list, temp_word);
        }
        ht_set(hashtable, temp_word, result + occurances);
        free(temp_word);
    }
}

//Checks if all the input and output files were opened successfully
bool check_files(const FILE *input_textfile, const FILE *output_countfile, const FILE *output_runtime){

    bool opened_successfully = true;

//    if (input_textfile == NULL){
//        fprintf(stderr, "Can't open input textfile. Please make sure the file exists and you have read permissions for it.\n");
//        opened_successfully = false;
//    }

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

void handle_input(int argc, char *argv[], FILE **input_textfile, FILE **output_countfile, FILE **output_runtime, int *num_processes){

    if (argc == 5){
//        *input_textfile = fopen(argv[1], "rb");
        *output_countfile = fopen(argv[2], "w");
        *output_runtime = fopen(argv[3], "w");
        *num_processes = atoi(argv[4]);
    } else{
        printf("Erroneous input supplied\n");
        printf("The program should be run with ./wordc-mp input_textfile output_countfile output_runtime number_of_processes\n");
        exit(1);
    }

    if (*num_processes < 1) {
        printf("Number of processes must be at least 1.\n");
        exit(1);
    }
}
