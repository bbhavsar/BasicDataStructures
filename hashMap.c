#include "hashMap.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BOOL    int
#define TRUE    1
#define FALSE   0

typedef struct node {
    char *key;
    void *val;
    struct node *prev;
    struct node *next;
} node;

struct HashMap {
    size_t size;        // number of buckets
    node **buckets;     
    size_t totalElems;  // elements currently stored in the HashMap
};

// Local functions.

// Get bucket index for given key.
//
// Function private to HashMap implementation
static int
getHashIndex(const HashMap *hm,     // IN
             const char *key)       // IN
{
    int val = 0;

    assert(key != NULL && hm != NULL);

    // Trivial hash function.
    for(; *key != '\0'; key++) {
        val += *key;
    }

    return val % hm->size;
}


// Return node in the HashMap for given key, if present.
// NULL ==> key is not present in the HashMap.
//
// Function private to HashMap implementation
static node *
lookupNode(const HashMap *hm,   // IN
           const char *key)     // IN
{
    assert(hm != NULL && key != NULL);
    node *list;
    const int bucketIndex = getHashIndex(hm, key);

    for (list = hm->buckets[bucketIndex]; list != NULL; list = list->next) {
        if (strcmp(list->key, key) == 0) {
            // Found the key!
            return list;
        }
    }

    return NULL;
}


// Global functions.

HashMap *
HashMap_create(const size_t size)  // IN
{
    HashMap *hm = malloc(sizeof *hm);
    if (hm == NULL) {
        return NULL;
    }

    hm->buckets = calloc(size, sizeof *hm->buckets);
    if (hm->buckets == NULL) {
        free(hm);
        return NULL;
    }

    hm->size = size;
    hm->totalElems = 0;
    return hm;  
}


size_t
HashMap_getNumElems(const HashMap *hm)    // IN
{
    return hm->totalElems;
}


// TRUE  ==> key is present in the HashMap and it's value will be set in *val.
// FALSE ==> key is not present in the HashMap
BOOL
HashMap_get(const HashMap *hm,      // IN
            const char *key,        // IN
            void **val)             // OUT
{
    node *n = lookupNode(hm, key);
    if (n != NULL) {
        *val = n->val;
        return TRUE;
    } else {
        *val = NULL;
        return FALSE;
    }
}


// 0  ==> key not present previously and new val successfully added
// 1  ==> key present previously and new val successfully updated
// -1 ==> error in key insertion and new val NOT added
int 
HashMap_put(HashMap *hm,            // IN
            const char *key,        // IN
            void *val,              // IN
            void **prevVal)         // OUT
{
    node *n = lookupNode(hm, key);
    if (n != NULL) {
        // Key already present in the HashMap,
        // so simply replace the old val.
        *prevVal = n->val;
        n->val = val;
        return 1;
    }

    // Add the new key at the head of the bucket.
    node *newNode = malloc(sizeof *newNode);
    if (newNode == NULL) {
        return -1;
    }
    newNode->key = strdup(key);
    newNode->val = val;
    newNode->prev = NULL;
    
    const int bucketIndex = getHashIndex(hm, key);
    node *prevBucketHead = hm->buckets[bucketIndex];
    if (prevBucketHead != NULL) {
        prevBucketHead->prev = newNode;
    }
    newNode->next = prevBucketHead;
    hm->buckets[bucketIndex] = newNode;

    hm->totalElems++;

    *prevVal = NULL;
    return 0;
}


// TRUE  ==> key found and removed successfully
// FALSE ==> key not found
BOOL
HashMap_removeEntry(HashMap *hm,        // IN
                    const char *key)    // IN
{
    node *n = lookupNode(hm, key);
    if (!n) {
        return FALSE;
    }

    node *prevNode = n->prev;
    node *nextNode = n->next;

    if (prevNode != NULL) {
        prevNode->next = nextNode;
    }

    if (nextNode != NULL) {
        nextNode->prev= prevNode;
    }

    // Check whether head node of bucket is being removed.
    {
        const int bucketHead = getHashIndex(hm, key);
        if (hm->buckets[bucketHead] == n) {
            assert(prevNode == NULL);
            hm->buckets[bucketHead] = nextNode;        
        }
    }

    free(n->key);
    free(n);

    hm->totalElems--;
    return TRUE;  
}


// Returns array of keys terminated by NULL.
char **
HashMap_getAllKeys(const HashMap *hm)     // IN
{
    char **result = calloc(hm->totalElems + 1, sizeof *result);
    int i;
    int num = 0;

    for (i = 0; i < hm->size; i++) {
        node *list;

        for (list = hm->buckets[i]; list != NULL; list = list->next) {
            assert(num < hm->totalElems);
            result[num++] = strdup(list->key);
        }
    }

    assert(num == hm->totalElems);
    result[num] = NULL;
    return result;
}


void
HashMap_freeKeys(char **keys)      // IN
{
    free(keys);
}


void
HashMap_delete(HashMap *hm)      // IN
{
    char **keys = HashMap_getAllKeys(hm);
    char **it;

    for (it = keys; *it != NULL; it++) {
        BOOL result = HashMap_removeEntry(hm, *it);
        assert(result);
    }

    assert(hm->totalElems == 0);
    free(hm->buckets);
    free(hm);

    HashMap_freeKeys(keys);
}

