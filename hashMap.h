/* 
 * Interface of a trivial hash table implementation in C.
 */

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include <stdio.h>

#ifndef BOOL

#define BOOL    int
#define TRUE    1
#define FALSE   0

#endif // BOOL

// Forward declarations to hide private members of HashMap implementation.

struct HashMap;
typedef struct HashMap HashMap;


// HashMap APIs.

// TODO: Use generic (void *) keys instead of string(char *).
// valueOfKey function should return numeric value of the key.
// Same keys should return same numeric value, 
// For e.g.
// char str[] = "dog";
// valueOfKey("dog") == valueOfKey(str)

typedef int (*valueOfKeyFptr)(void *key);

// Returns the HashMap.
// NULL ==> Error in creating HashMap
HashMap *
HashMap_create(const size_t size);   // IN

// Delete HashMap created by create() function.
void
HashMap_delete(HashMap *hm);      // IN

// Fetch value associated with the key, if present.
// TRUE  ==> key is present in the HashMap and it's value will be set in *val.
// FALSE ==> key is not present in the HashMap
BOOL
HashMap_get(const HashMap *hm,      // IN
            const char *key,        // IN
            void **val);            // OUT

// Insert the key --> value pair in the HashMap.
//  0  ==> key not present previously and new val successfully added
//  1  ==> key present previously and new val successfully updated
// -1  ==> error in key insertion and new val NOT added
int
HashMap_put(HashMap *hm,        // IN
            const char *key,    // IN
            void *val,          // IN
            void **prevVal);    // OUT

// Remove key --> value pair from the HashMap, if present. 
// TRUE  ==> key found and removed successfully
// FALSE ==> key not found
BOOL
HashMap_removeEntry(HashMap *hm,        // IN
                    const char *key);   // IN

// Returns array of keys terminated by NULL.
// Caller should use deleteAllKeys() to release
// keys returned by this function.
char **
HashMap_getAllKeys(const HashMap *hm);      // IN

// Releases memory allocated for keys returned by getAllKeys() function.
void
HashMap_freeKeys(char **keys);     // IN

// Returns number of key --> value pairs currently stored in the HashMap.
size_t
HashMap_getNumElems(const HashMap *hm);    // IN

#endif // _HASH_MAP_H_

