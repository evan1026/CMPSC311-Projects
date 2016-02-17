#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/************************************************
 * This file contains only basic documentation. *
 * For full code documentation, please see the  *
 * corresponding .c file                        *
 ************************************************/

// One entry in the hash table linked list
// Each entry knows its key, the count, and the next one in the list
struct entry_s {
    char *key;
    int value;
    struct entry_s *next;
};

typedef struct entry_s entry_t;

// The hashtable itself knows how big it is and contains
// an array of pointers to entries
struct hashtable_s {
    int size;
    struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

// For docs on each function, look in hashtable.c
hashtable_t *ht_create( const int size );
int ht_hash( hashtable_t *hashtable, const char *key );
entry_t *ht_newpair( const char *key, const int value );
void ht_set( hashtable_t *hashtable, const char *key, const int value );
int ht_get( hashtable_t *hashtable, const char *key );
void ht_dispose( hashtable_t *hashtable );

#endif
