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
#include <stdbool.h>
#include <pthread.h>

#include "mapreduce.h"


/* Size of shared memory buffers */
#define MR_BUFFER_SIZE 1024


/* Allocates and initializes an instance of the MapReduce framework */
struct map_reduce *
mr_create(map_fn map, reduce_fn reduce, int threads)
{
	bool error = false;
	int i, temp = 0;
	struct map_reduce *mr = (struct map_reduce *) malloc(sizeof(struct map_reduce));

	//First, let's make sure we got something
	if (mr == NULL) {
		//This variable will be used for cleanup later
		error = true;
	} else {
		//Okay, we got the struct. Let's start initializing
		mr->mapfn = map;
		mr->reducefn = reduce;
		mr->num_threads = threads;
		mr->buffers = (struct kvpair **) malloc(threads * sizeof(struct kvpair *));
		mr->buf_mutexes = (pthread_mutex_t *) malloc(threads * sizeof(pthread_mutex_t));
		mr->threads = (pthread_t *) malloc(threads * sizeof(pthread_t));

		//Let's make sure the buffer array was created
		if (mr->buffers == NULL) {
			error = true;
		} else {
			//And now we'll initialize each individual buffer
			for (i = 0; i < threads; ++i) {
				//Initialize the current buffer
				mr->buffers[i] = (struct kvpair *) malloc(MR_BUFFER_SIZE * sizeof(struct kvpair));

				//If this one failed, no point in continuing
				if (mr->buffers[i] == NULL) break;
			}

			//If we didn't make it to the end, then the current one must have failed
			if (i != threads) {
				//So we'll back up and clean up the ones that previously succeeded
				for (--i; i >= 0; --i) {
					free(mr->buffers[i]);
				}
				error = true;
			}
		}

		//Now, we'll do the same thing with the mutexes
		if (mr->buf_mutexes == NULL) {
			error = true;
		} else {
			//Array allocated, so let's start initialization
			for (i = 0; i < threads; ++i) {
				temp = pthread_mutex_init(&mr->buf_mutexes[i], NULL);

				//If it returns non-zero, it failed, so we stop trying and work on cleanup
				if (temp != 0) break;
			}

			//If the previous one failed....
			if (temp != 0) {
				//Back up a bit and free up the ones that succeeded
				for(--i; i >= 0; --i) {
					pthread_mutex_destroy(&mr->buf_mutexes[i]);
				}
				error = true;
			}
		}

		//For this one, we don't want to initialize quite yet
		//But we will make sure the memory is available to us
		if (mr->threads == NULL) {
			error = true;
		}
	}

	//If anything failed, we need to clean up the memory and return NULL
	if (error) {
		//This is all pretty self-explainatory, but to summarize, if
		//we have a valid pointer, we must free it
		if (mr != NULL) {
			if (mr->buffers != NULL)
				free(mr->buffers);
			if (mr->buf_mutexes != NULL)
				free(mr->buf_mutexes);
			if (mr->threads != NULL)
				free(mr->threads);
			free(mr);
		}
		return NULL;
	}

	//If we're here, everything worked, and we can return the successfully
	//created object
	return mr;
}

/* Destroys and cleans up an existing instance of the MapReduce framework */
void
mr_destroy(struct map_reduce *mr)
{
	int i;

	//I think the code here mostly speaks for itself;
	//it checks to see if there is memory allocated
	//for each part of the struct, and frees it if so
	if (mr != NULL) {
		if (mr->buffers != NULL) {
			for (i = 0; i < mr->num_threads; ++i) {
				free(mr->buffers[i]);
			}
			free(mr->buffers);
		}
		if (mr->buf_mutexes != NULL) {
			for (i = 0; i < mr->num_threads; ++i) {
				pthread_mutex_destroy(&mr->buf_mutexes[i]);
			}
			free(mr->buf_mutexes);
		}
		if (mr->threads != NULL) {
			free(mr->threads);
		}
		free(mr);
	}
}

/* Begins a multithreaded MapReduce operation */
int
mr_start(struct map_reduce *mr, const char *inpath, const char *outpath)
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
