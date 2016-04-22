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
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "mapreduce.h"


/* Size of shared memory buffers */
#define MR_BUFFER_SIZE 10240 

struct map_params{
	struct map_reduce *mr;
	int infd;
	int id;
};

//Fills the map_params struct with the appropriate data
static struct map_params *setup_map_params(struct map_reduce *mr, const char *inpath, int id){
	struct map_params *mp = malloc(sizeof(struct map_params));

	if (mp == NULL) return NULL;

	mp->mr = mr;
	mp->id = id;
	mp->infd = open(inpath, O_RDONLY);

	if (mp->infd == -1){
		fprintf(stderr, "Could not open input file, please make sure the file exists and that you have read permission for it\n");
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
mr_create(map_fn map, reduce_fn reduce, int threads)
{
	bool error = false;
	int i, temp = 0;
	struct map_reduce *mr = malloc(sizeof(struct map_reduce));

	//First, let's make sure we got something
	if (mr == NULL) {
		//This variable will be used for cleanup later
		error = true;
	} else {
		//Okay, we got the struct. Let's start initializing
		mr->mapfn = map;
		mr->reducefn = reduce;
		mr->num_threads = threads;
		mr->buffers = malloc(threads * sizeof(struct kvpair *));
		mr->buf_mutexes = malloc(threads * sizeof(pthread_mutex_t));
		mr->in = malloc(threads * sizeof(int));                               //I really like the way these
		mr->out = malloc(threads * sizeof(int));                              //malloc's happened to line up
		mr->count = malloc(threads * sizeof(int));
		mr->finished = malloc(threads * sizeof(bool));
		mr->map_threads = malloc(threads * sizeof(pthread_t *));
		mr->reduce_thread = malloc(sizeof(pthread_t));

		//Let's make sure the buffer array was created
		if (mr->buffers == NULL) {
			error = true;
		} else {
			//And now we'll initialize each individual buffer
			for (i = 0; i < threads; ++i) {
				//Initialize the current buffer
				mr->buffers[i] = malloc(MR_BUFFER_SIZE * sizeof(struct kvpair));

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

		//Now we initialize in 'in' variable which keeps track of where the mapper is
		//in its buffer
		if (mr->in == NULL) {
			error = true;
		} else {
			for (i = 0; i < threads; ++i) {
				mr->in[i] = 0;
			}
		}

		//And then the 'out' variable, which keeps track of where the reducer is
		//in each buffer
		if (mr->out == NULL) {
			error = true;
		} else {
			for (i = 0; i < threads; ++i) {
				mr->out[i] = 0;
			}
		}

		//Now for count, which counts the number of objects in the buffer
		if (mr->count == NULL) {
			error = true;
		} else {
			for (i = 0; i < threads; ++i) {
				mr->count[i] = 0;
			}
		}

		//And finished, which keeps track of which threads have finished execution
		if (mr->finished == NULL) {
			error = true;
		} else {
			for (i = 0; i < threads; ++i) {
				mr->finished[i] = false;
			}
		}

		//Now, we'll do the mutexes
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
		if (mr->map_threads == NULL) {
			error = true;
		} else {
			for (i = 0; i < threads; ++i) {
				mr->map_threads[i] = malloc(sizeof(pthread_t));

				if (mr->map_threads[i] == NULL) break;
			}

			if (i != threads) {
				for (--i; i >= 0; --i) {
					free(mr->map_threads[i]);
				}
				error = true;
			}
		}

		if (mr->reduce_thread == NULL) {
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
			if (mr->in != NULL)
				free(mr->in);
			if (mr->out != NULL)
				free(mr->out);
			if (mr->count != NULL)
				free(mr->count);
			if (mr->finished != NULL)
				free(mr->finished);
			if (mr->buf_mutexes != NULL)
				free(mr->buf_mutexes);
			if (mr->map_threads != NULL)
				free(mr->map_threads);
			if (mr->reduce_thread != NULL)
				free(mr->reduce_thread);
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
		if (mr->in != NULL) {
			free(mr->in);
	   	}
		if (mr->out != NULL) {
			free(mr->out);
		}
		if (mr->count != NULL) {
			free(mr->count);
		}
		if (mr->finished != NULL) {
			free(mr->finished);
		}
		if (mr->buf_mutexes != NULL) {
			for (i = 0; i < mr->num_threads; ++i) {
				pthread_mutex_destroy(&mr->buf_mutexes[i]);
			}
			free(mr->buf_mutexes);
		}
		if (mr->map_threads != NULL) {
			for (i = 0; i < mr->num_threads; ++i) {
				if (mr->map_threads[i] != NULL)
					free(mr->map_threads[i]);
			}
			free(mr->map_threads);
		}
		if (mr->reduce_thread != NULL) {
			free(mr->reduce_thread);
		}
		free(mr);
	}
}

/* Begins a multithreaded MapReduce operation */
int
mr_start(struct map_reduce *mr, const char *inpath, const char *outpath)
{
	if (mr == NULL || inpath == NULL || outpath == NULL)
		return 1;

	int output_fd = open(outpath, O_WRONLY | O_CREAT | O_TRUNC);
	int infd = open(inpath, O_RDONLY);
	bool error = false;

	//Check if output file was opened correctly
	if (output_fd == -1 || infd == -1){
		fprintf(stderr, "Could not open or create file\n");
		perror("");

		free(mr->reduce_thread);
		mr->reduce_thread = NULL;

		for (int i = 0; i < mr->num_threads; i++) {
			free(mr->map_threads[i]);
			mr->map_threads[i] = NULL;
		}

		if (output_fd != -1)
			close(output_fd);

		if (infd != -1)
			close(infd);

		return 1;
	}

	close(infd);

	if (mr->num_threads >= 1){
		for (int i = 0; i < mr->num_threads; i++){
			struct map_params *mp = setup_map_params(mr, inpath, i);

			//If setup_map_params could not correctly set up our map params struct
			if (mp == NULL){
				fprintf(stderr, "Error setting up map params struct. Skipping creation of thread%d\n", i);
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

		for (int i = 0; i < mr->num_threads; i++) {
			free(mr->map_threads[i]);
			mr->map_threads[i] = NULL;
		}

		close(output_fd);

		return 1;
	}

	struct reduce_params *rp = setup_reduce_params(mr, output_fd);
	if (rp == NULL) {
		error = true;
		if (close(output_fd) != 0) {
			fprintf(stderr, "Error closing output file\n");
			perror("");
		}
		free(mr->reduce_thread);
		mr->reduce_thread = NULL;
	} else {
		if (pthread_create(mr->reduce_thread, NULL, run_reduce, rp) != 0){
			fprintf(stderr, "Error creating reduce thread\n");
			perror("");
			free(mr->reduce_thread);
			mr->reduce_thread = NULL;
			error = true;
			close(output_fd);
		}
	}

	if (error) return 1;

	return 0;
}

/* Blocks until the entire MapReduce operation is complete */
int
mr_finish(struct map_reduce *mr)
{
	if (mr == NULL)
		return -1;

	int i, out = 0;
	void *ret = NULL;
	int *ret_i;

	//Go through and wait on each thread
	for (i = 0; i < mr->num_threads; ++i) {
		if (mr->map_threads[i] == NULL)
			continue;

		//Grab the return while waiting
		pthread_join(*mr->map_threads[i], &ret);
		ret_i = ret;

		//If no return, something went wrong, so keep track of it, but keep waiting
		if (ret_i == NULL) {
			out = 1;
			continue; //So we don't run the free later
		}

		//If the return wasn't zero, that's also an issue
		if (*ret_i != 0)
			out = 1;

		free(ret_i);
	}

	//Do the same stuff, but with the reducer
	if (mr->reduce_thread != NULL){
		pthread_join(*mr->reduce_thread, &ret);
		ret_i = ret;

		if (ret_i == NULL) {
			out = 1;
		} else {
			if (*ret_i != 0)
				out = 1;
			free(ret_i);
		}
	}

	//If anything went wrong, this will return 1, 0 otherwise
	return out;
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
	if (kvcopy.key == NULL || kvcopy.value == NULL) {
		if (kvcopy.key != NULL)
			free(kvcopy.key);
		if (kvcopy.value != NULL)
			free(kvcopy.value);
		return -1;
	}

	//Copy the data
	memcpy(kvcopy.key, kv->key, kvcopy.keysz);
	memcpy(kvcopy.value, kv->value, kvcopy.valuesz);

	//Wait for the mutex to be available, and then put it in the buffer
	while (1) {
		pthread_mutex_lock(&mr->buf_mutexes[id]); //Wait on mutex
		if (mr->count[id] < MR_BUFFER_SIZE) { //Make sure there's space
			mr->buffers[id][mr->in[id]] = kvcopy; //Put in the data
			mr->in[id] = (mr->in[id] + 1) % MR_BUFFER_SIZE; //Change the in value
			mr->count[id]++; //And the count
			break;
		}
		pthread_mutex_unlock(&mr->buf_mutexes[id]); //Release the mutex if we didn't update
	}
	pthread_mutex_unlock(&mr->buf_mutexes[id]); //Release the mutex if we did update

	return 1;
}

/* Called by the Reduce function to consume a key-value pair */
int
mr_consume(struct map_reduce *mr, int id, struct kvpair *kv)
{
	struct kvpair *buf_kv;
	if (kv == NULL || mr == NULL)
		return -1;

	//Wait for the mutex to become available, and then copy the data out and into
	//the struct that has been allocated for us
	while (1) {
		pthread_mutex_lock(&mr->buf_mutexes[id]);
		buf_kv = &mr->buffers[id][mr->out[id]];
		if (mr->count[id] > 0) {
			if (kv->keysz < buf_kv->keysz || kv->valuesz < buf_kv->valuesz) {
				//not enough space allocated => error
				pthread_mutex_unlock(&mr->buf_mutexes[id]); //Almost forgot this because I'm used to std::lock_guard
				return -1;
			}
			//Ok, we have space. Let's do the copy
			memcpy(kv->key, buf_kv->key, buf_kv->keysz);
			memcpy(kv->value, buf_kv->value, buf_kv->valuesz);
			kv->keysz = buf_kv->keysz;
			kv->valuesz = buf_kv->valuesz;

			//Mark the space we were using for the data as freed
			free(buf_kv->key);
			free(buf_kv->value);

			//And do the accounting
			mr->out[id] = (mr->out[id] + 1) % MR_BUFFER_SIZE;
			mr->count[id]--;
			pthread_mutex_unlock(&mr->buf_mutexes[id]);
			return 1;
		} else if (mr->finished[id]) { //Else means nothing in the buffer, let's see if it's because the thread is done
			//Ok, thread is done. Return empty kvpair
			kv->keysz = 0;
			kv->valuesz = 0;
			pthread_mutex_unlock(&mr->buf_mutexes[id]);
			return 0;
		}
		pthread_mutex_unlock(&mr->buf_mutexes[id]);
	}
	pthread_mutex_unlock(&mr->buf_mutexes[id]);

	return -1; //Shouldn't even get here anyway
}
