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
	return NULL;
}

/* Destroys and cleans up an existing instance of the MapReduce framework */
void
mr_destroy(struct map_reduce *mr)
{
}

/* Begins a multithreaded MapReduce operation */
int
mr_start(struct map_reduce *mr, const char *path, const char *ip, uint16_t port)
{
	return 0;
}

/* Blocks until the entire MapReduce operation is complete */
int
mr_finish(struct map_reduce *mr)
{
	return 0;
}

/* Called by the Map function each time it produces a key-value pair */
int
mr_produce(struct map_reduce *mr, int id, const struct kvpair *kv)
{
	return 0;
}

/* Called by the Reduce function to consume a key-value pair */
int
mr_consume(struct map_reduce *mr, int id, struct kvpair *kv)
{
	return 0;
}
