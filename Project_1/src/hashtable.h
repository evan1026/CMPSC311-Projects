#ifndef HASH_TABLE_H
#define HASH_TABLE_H

struct entry_s {
    char *key;
    int value;
    struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
    int size;
    struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create( const int size );
int ht_hash( hashtable_t *hashtable, const char *key );
entry_t *ht_newpair( const char *key, const int value );
void ht_set( hashtable_t *hashtable, const char *key, const int value );
int ht_get( hashtable_t *hashtable, const char *key );
void ht_dispose( hashtable_t *hashtable );

#endif
