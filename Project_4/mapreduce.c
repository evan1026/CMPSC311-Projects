/*
 * Implementation file for simple MapReduce framework.  Fill in the functions
 * and definitions in this file to complete the assignment.
 *
 * Place all of your implementation code in this file.  You are encouraged to
 * create helper functions wherever it is necessary or will make your code
 * clearer.  For these functions, you should follow the practice of declaring
 * them "static" and not including them in the header file (which should only be
 * used for the *public-facing* API.
 */


/* Header includes */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "mapreduce.h"


/* Size of shared memory buffers */
#define MR_BUFFER_SIZE 1024


struct map_params{
    struct map_reduce *mr;
    int infd;
    int id;
};

//Fills the map_params struct with the appropriate data
static struct map_params *setup_map_params(struct map_reduce *mr, const char *inpath, int id  ){
    struct map_params *mp = malloc(sizeof(struct map_params));

    if (mp == NULL) return NULL;

    mp->mr = mr;
    mp->id = id;
    mp->infd = open(inpath, O_RDONLY);

    if (mp->infd == -1){
	fprintf(stderr, "Could not open input file, please make sure the file exists and tha  t you have read permission for it\n");
        perror("");
        free(mp);
        return NULL;
    } else {
	return mp;
    }
}

struct reduce_params {
    struct map_reduce *mr;
    int outfd;
};

//Fills the reduce_params struct with the appropriate data
static struct reduce_params *setup_reduce_params(struct map_reduce *mr, int outfd){
    struct reduce_params *rp = malloc(sizeof(struct map_params));

    if (rp == NULL) return NULL;

    rp->mr = mr;
    rp->outfd = outfd;

    return rp;
}

//Gets called as the first function of the new thread
//passes args on to mapper function
static void *run_map(void *args){
    struct map_params *mp = args;
    int *ret = malloc(sizeof(int));

    if (ret == NULL)
        return NULL;

    *ret = mp->mr->mapfn(mp->mr, mp->infd, mp->id, mp->mr->num_threads);

    //Marks as finished; value used to signal reducer that there is nothing left
    mp->mr->finished[mp->id] = true;

    //Check if input file is closed correctly
    if (close(mp->infd) == -1){
        fprintf(stderr, "Error closing input file for thread %d\n", mp->id);
        perror("");
    }

    free(mp);

    return ret;
  }

//Gets called as the first function of the new thread
//passes args on to reducer function
static void *run_reduce(void *args) {
    struct reduce_params *rp = args;
    int *ret = malloc(sizeof(int));

    if (ret == NULL)
        return NULL;

    *ret = rp->mr->reducefn(rp->mr, rp->outfd, rp->mr->num_threads);

    //Check if input file is closed correctly
    if (close(rp->outfd) == -1){
        fprintf(stderr, "Error closing output file\n");
        perror("");
    }

    free(rp);

    return ret;
}

/* Allocates and initializes an instance of the MapReduce framework */
struct map_reduce *
mr_create(map_fn map, reduce_fn reduce, int nmaps)
{
    struct map_reduce *mr = malloc(sizeof(struct map_reduce));

    if (mr != NULL) {

        mr->num_threads = nmaps;
        mr->mapfn = map;
        mr->reducefn = reduce;

        mr->sockfd = malloc(nmaps * sizeof(int));
        mr->socket = malloc(nmaps * sizeof(struct sockaddr_in));

        if (map == NULL) {
            //Server
            if (reduce != NULL) {
                mr->is_server = true;

                mr->finished      = malloc(nmaps * sizeof(bool));
                mr->map_threads   = NULL;
                mr->reduce_thread = malloc(sizeof(pthread_t));

                if (mr->sockfd != NULL && mr->socket != NULL && mr->finished != NULL && mr->reduce_thread != NULL) {
                    return mr;
                }

                //No need for else, we'll just end up at the part later where everything is freed
            }
        } else if (reduce == NULL) {
            //Client

            mr->is_server = false;

            mr->finished = NULL;
            mr->reduce_thread = NULL;
            mr->map_threads = malloc(nmaps * sizeof(pthread_t *));

            if (mr->map_threads != NULL){
                int i;

                for (i = 0; i < nmaps; ++i){
                    mr->map_threads[i] = malloc(sizeof(pthread_t));

                    if (mr->map_threads[i] == NULL)
                        break;
                }

                if (i != nmaps){
                    for (--i; i >= 0; --i){
                        free(mr->map_threads[i]);
                    }
                    return NULL;
                }
            }

            if (mr->sockfd != NULL && mr->socket != NULL && mr->map_threads != NULL) {
                return mr;
            }

        }
    }

    //If here, there is an error
    if (mr != NULL){
        if (mr->sockfd != NULL)
            free(mr->sockfd);
        if (mr->socket != NULL)
            free(mr->socket);
        if (mr->finished != NULL)
            free(mr->finished);
        if (mr->map_threads != NULL)
            free(mr->map_threads);
        if (mr->reduce_thread != NULL)
            free(mr->reduce_thread);
        free(mr);
    }

    return NULL;
}

/* Destroys and cleans up an existing instance of the MapReduce framework */
void
mr_destroy(struct map_reduce *mr)
{
    if (mr != NULL){
        if (mr->sockfd != NULL)
            free(mr->sockfd);
        if (mr->socket != NULL)
            free(mr->socket);
        if (mr->finished != NULL)
            free(mr->finished);
        if (mr->map_threads != NULL){
            for (int i = 0; i < mr->num_threads; i++){
                if (mr->map_threads[i] != NULL)
                    free(mr->map_threads[i]);
            }
            free(mr->map_threads);
        }
        if (mr->reduce_thread != NULL)
            free(mr->reduce_thread);
        free(mr);
    }
}

/* Begins a multithreaded MapReduce operation */
int
mr_start(struct map_reduce *mr, const char *path, const char *ip, uint16_t port)
{
    if (mr == NULL || path == NULL || ip == NULL)
        return 1;

    int fd;
    bool error = false;

    if (mr->is_server){
        fd = open(path, O_WRONLY | O_CREAT | O_TRUNC);
    } else {
        fd = open(path, O_RDONLY);
    }

    if (fd == -1){
        fprintf(stderr, "Could not open or create file\n");
        perror("");

        free(mr->reduce_thread);
        mr->reduce_thread = NULL;

        for(int i = 0; i < mr->num_threads; i++){
            free(mr->map_threads[i]);
            mr->map_threads[i] = NULL;
        }

        return 1;
    }

    if (mr->is_server){
        //Referenced http://www.gnu.org/software/libc/manual/html_node/Inet-Example.html#Inet-Example

        *mr->sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (*mr->sockfd == -1){
            fprintf(stderr, "Error creating socket\n");
            perror("");
        }

        mr->socket->sin_family = AF_INET;
        mr->socket->sin_port = htons(port);
        mr->socket->sin_addr.s_addr = htonl (INADDR_ANY);

        if (bind(*mr->sockfd, (struct sockaddr *) mr->socket, sizeof (*mr->socket)) == -1){
            fprintf(stderr, "Error binding socket\n");
            perror("");
        }

        if (listen(*mr->sockfd, SOMAXCONN) == -1){ //Should SOMAXCONN be used here?
            fprintf(stderr, "Error listening on socket\n");
            perror("");
        }

        //TODO
        /* struct reduce_params *rp = setup_reduce_params(mr, fd);
        if (rp == NULL){
            fprintf(stderr, "Error setting up reduce params struct\n");
            error = true;
            free(mr->reduce_thread);
            mr->reduce_thread = NULL;
            if (close(fd) != 0){
                fprintf(stderr, "Error closing output file\n");
                perror("");
            }
        } else {
            if (pthread_create(mr->reduce_thread, NULL, run_reduce, rp) != 0){
                fprintf(stderr, "Error creating reduce thread\n");
                perror("");
                free(mr->reduce_thread);
                mr->reduce_thread = NULL;
                error = true;
                close(fd);
            }
        } */

    } else {
        //Reaches here if client
        close(fd);

        if (mr->num_threads >= 1){
            for (int i = 0; i < mr->num_threads; i++){
                struct map_params *mp = setup_map_params(mr, path, i);

                mr->sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);

                if (mr->sockfd[i] == -1){
                    fprintf(stderr, "Error creating socket\n");
                    perror("");
                }

                mr->socket->sin_family = AF_INET;
                mr->socket->sin_port = htons(port);
                mr->socket->sin_addr.s_addr = htonl(INADDR_ANY);

                if (connect(*mr->sockfd, (struct sockaddr *) mr->socket, sizeof(*mr->socket)) == -1){
                    fprintf(stderr, "Error connecting thread %d\n", i);
                    perror("");
                }

                //If setup_map_params could not correctly set up our map params struct
                if (mp == NULL){
                    fprintf(stderr, "Error setting up map params struct. Skipping creation of thread %d\n", i);
                    error = true;
                    free(mr->map_threads[i]);
                    mr->map_threads[i] = NULL;
                } else {
                    if (pthread_create(mr->map_threads[i], NULL, run_map, mp) != 0){
                        fprintf(stderr, "Error creating map thread %d\n", i);
                        perror("");
                        error = true;
                        free(mr->map_threads[i]);
                        mr->map_threads[i] = NULL;
                    }
                }
            }
        } else {
            fprintf(stderr, "Cannot run with less than 1 thread\n");

            free(mr->reduce_thread);
            mr->reduce_thread = NULL;

            for(int i = 0; i < mr->num_threads; i++){
                free(mr->map_threads[i]);
                mr->map_threads[i] = NULL;
            }

            return 1;
        }
    }

    if (error)
        return 1;

    return 0;
}

/* Blocks until the entire MapReduce operation is complete */
int
mr_finish(struct map_reduce *mr)
{
    if (mr == NULL)
        return -1;

    return 0;
}

/* Called by the Map function each time it produces a key-value pair */
int
mr_produce(struct map_reduce *mr, int id, const struct kvpair *kv)
{
    if (kv == NULL || mr == NULL)
        return -1;

    //Make some space to copy the values
    struct kvpair kvcopy;
    kvcopy.key = malloc(kv->keysz);
    kvcopy.value = malloc(kv->valuesz);
    kvcopy.keysz = kv->keysz;
    kvcopy.valuesz = kv->valuesz;

    //Make sure we got the space
    if (kvcopy.key == NULL || kvcopy.value == NULL){
        if (kvcopy.key != NULL)
            free(kvcopy.key);
        if (kvcopy.value != NULL)
            free(kvcopy.value);
        return -1;
    }

    //Copy the data
    memcpy(kvcopy.key, kv->key, kvcopy.keysz);
    memcpy(kvcopy.value, kv->value, kvcopy.valuesz);

    return 0;
}

/* Called by the Reduce function to consume a key-value pair */
int
mr_consume(struct map_reduce *mr, int id, struct kvpair *kv)
{
    if (kv == NULL || mr == NULL)
        return -1;

    return 0;
}
