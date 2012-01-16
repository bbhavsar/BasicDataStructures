/* 
 * Interface of a trivial hash table implementation in C.
 */

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdio.h>

#ifndef BOOL

#define BOOL    int
#define TRUE    1
#define FALSE   0

#endif // BOOL

// Forward declarations to hide private members of hashtable implementation.

struct node;
typedef struct node node;

struct hashtable;
typedef struct hashtable hashtable;


// Hashtable APIs.

// TODO: Use generic (void *) keys instead of string(char *).
// valueOfKey function should return numeric value of the key.
// Same keys should return same numeric value, 
// For e.g.
// char str[] = "dog";
// valueOfKey("dog") == valueOfKey(str)

typedef int (*valueOfKeyFptr)(void *key);

// Returns the hashtable.
// NULL ==> Error in creating hashtable
hashtable *
createHashtable(const size_t size);   // IN

// Delete hashtable created by createHashTable() function.
void
deleteHashTable(hashtable *ht);      // IN

// Fetch value associated with the key, if present.
// TRUE  ==> key is present in the hashtable and it's value will be set in *val.
// FALSE ==> key is not present in the hashtable
BOOL
get(const hashtable *ht,        // IN
    const char *key,            // IN
    void **val);                // OUT

// Insert the key --> value pair in the hashtable.
//  0  ==> key not present previously and new val successfully added
//  1  ==> key present previously and new val successfully updated
// -1  ==> error in key insertion and new val NOT added
int
put(hashtable *ht,              // IN
    const char *key,            // IN
    void *val,                  // IN
    void **prevVal);            // OUT

// Remove key --> value pair from the hashtable, if present. 
// TRUE  ==> key found and removed successfully
// FALSE ==> key not found
BOOL
removeEntry(hashtable *ht,
            const char *key);

// Returns array of keys terminated by NULL.
// Caller should use deleteAllKeys() to release
// keys returned by this function.
char **
getAllKeys(const hashtable *ht);      // IN

// Deletes keys returned by getAllKeys() function.
void
deleteAllKeys(char **keys);     // IN

// Returns number of key --> value pairs currently stored in the hashtable.
size_t
getNumElems(const hashtable *ht);    // IN

#endif // _HASH_TABLE_H_

