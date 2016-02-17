#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

// this file heavily based on https://gist.github.com/tonious/1377667

/* Create a new hashtable. */
hashtable_t *ht_create( const int size ) {

    hashtable_t *hashtable = NULL;
    int i;

    if( size < 1 ) return NULL;

    /* Allocate the table itself. */
    if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
        return NULL;
    }

    /* Allocate pointers to the head nodes. */
    if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
        return NULL;
    }
    for( i = 0; i < size; i++ ) {
        hashtable->table[i] = NULL;
    }

    hashtable->size = size;

    return hashtable;
}

/* Hash a string for a particular hash table. */
// hash from http://stackoverflow.com/questions/7666509/hash-function-for-string
int ht_hash( hashtable_t *hashtable, const char *key ) {

    unsigned long int hashval = 5381;
    int c;

    while ((c = *key++)) {
        hashval = ((hashval << 5) + hashval) + c; /* hash * 33 + c */
    }

    return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( const char *key, const int value ) {
    entry_t *newpair;

    if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
        return NULL;
    }

    if ((newpair->key = strdup(key)) == NULL) {
        free(newpair);
        return NULL;
    }
    newpair->value = 1;
    newpair->next = NULL;

    return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, const char *key, const int value ) {
    int bin = 0;
    entry_t *newpair = NULL;
    entry_t *next = NULL;
    entry_t *last = NULL;

    bin = ht_hash( hashtable, key );

    next = hashtable->table[ bin ];

    while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
        last = next;
        next = next->next;
    }

    /* There's already a pair.  Let's replace that string. */
    if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

        next->value = value;

    /* Nope, could't find it.  Time to grow a pair. */
    } else {
        newpair = ht_newpair( key, value );

        /* We're at the start of the linked list in this bin. */
        if( next == hashtable->table[ bin ] ) {
            newpair->next = next;
            hashtable->table[ bin ] = newpair;

        /* We're at the end of the linked list in this bin. */
        } else if ( next == NULL ) {
            last->next = newpair;

        /* We're in the middle of the list. */
        } else  {
            newpair->next = next;
            last->next = newpair;
        }
    }
}

/* Retrieve a key-value pair from a hash table. */
int ht_get( hashtable_t *hashtable, const char *key ) {
    int bin = 0;
    entry_t *pair;

    bin = ht_hash( hashtable, key );

    /* Step through the bin, looking for our value. */
    pair = hashtable->table[ bin ];
    while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
        pair = pair->next;
    }

    /* Did we actually find anything? */
    if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
        return -1;

    } else {
        return pair->value;
    }

}

/* Free up the memory in a hashtable */
void ht_dispose( hashtable_t *hashtable ) {
    if (hashtable == NULL) return;

    entry_t *temp1, *temp2;

    for (int i = 0; i < hashtable->size; ++i) {
        temp1 = hashtable->table[i];
        while (temp1 != NULL) {
            temp2 = temp1;
            temp1 = temp1->next;
            if (temp2->key != NULL) free(temp2->key);
            free(temp2);
        }
    }

    if (hashtable->table != NULL) free(hashtable->table);
    free(hashtable);
}
