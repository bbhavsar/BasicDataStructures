#include "hashtable.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BOOL    int
#define TRUE    1
#define FALSE   0

struct node {
    char *key;
    void *val;
    struct node *prev;
    struct node *next;
};

struct hashtable {
    size_t size;        // number of buckets
    node **buckets;     
    size_t totalElems;  // elements currently stored in the hashtable
};


hashtable *
createHashtable(const size_t size)  // IN
{
    hashtable *ht = malloc(sizeof *ht);
    if (ht == NULL) {
        return NULL;
    }

    ht->buckets = calloc(size, sizeof *ht->buckets);
    if (ht->buckets == NULL) {
        free(ht);
        return NULL;
    }

    ht->size = size;
    ht->totalElems = 0;
    return ht;  
}


size_t
getNumElems(const hashtable *ht)    // IN
{
    return ht->totalElems;
}

// Get bucket index for given key.
//
// Function private to hashtable implementation
static int
getHashIndex(const hashtable *ht,   // IN
             const char *key)       // IN
{
    int val = 0;

    assert(key != NULL && ht != NULL);

    // Trivial hash function.
    while (*key != '\0') {
        val += *key;
        key++;        
    }

    return val % ht->size;
}


// Return node in the hashtable for given key, if present.
// NULL ==> key is not present in the hashtable.
//
// Function private to hashtable implementation
static node *
lookupNode(const hashtable *ht,         // IN
           const char *key)             // IN
{
    assert(ht != NULL && key != NULL);

    int bucketIndex = getHashIndex(ht, key);
    node *list = ht->buckets[bucketIndex];
    while (list != NULL) {
        if (strcmp(list->key, key) == 0) {
            // Found the key!
            return list;
        }
        list = list->next;
    }

    return NULL;
}


// TRUE  ==> key is present in the hashtable and it's value will be set in *val.
// FALSE ==> key is not present in the hashtable
BOOL
get(const hashtable *ht,    // IN
    const char *key,        // IN
    void **val)             // OUT
{
    node *n = lookupNode(ht, key);
    if (n != NULL) {
        *val = n->val;
        return TRUE;
    } else {
        return FALSE;
    }
}

// 0  ==> key not present previously and new val successfully added
// 1  ==> key present previously and new val successfully updated
// -1 ==> error in key insertion and new val NOT added
int 
put(hashtable *ht,          // IN
    const char *key,        // IN
    void *val,              // IN
    void **prevVal)         // OUT
{
    node *n = lookupNode(ht, key);
    if (n != NULL) {
        // Key already present in the hashtable,
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
    
    const int bucketIndex = getHashIndex(ht, key);
    node *prevBucketHead = ht->buckets[bucketIndex];
    if (prevBucketHead != NULL) {
        prevBucketHead->prev = newNode;
    }
    newNode->next = prevBucketHead;
    ht->buckets[bucketIndex] = newNode;

    ht->totalElems++;

    *prevVal = NULL;
    return 0;
}


// TRUE  ==> key found and removed successfully
// FALSE ==> key not found
BOOL
removeEntry(hashtable *ht,      // IN
            const char *key)    // IN
{
    node *n = lookupNode(ht, key);
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
    int bucketHead = getHashIndex(ht, key);
    if (ht->buckets[bucketHead] == n) {
        assert(prevNode == NULL);
        ht->buckets[bucketHead] = nextNode;        
    }

    free(n->key);
    free(n);

    ht->totalElems--;
    return TRUE;  
}


// Returns array of keys terminated by NULL.
char **
getAllKeys(hashtable *ht)
{
    char **result = calloc(ht->totalElems + 1, sizeof *result);
    int i;
    int num = 0;

    for (i = 0; i < ht->size; i++) {
        node *list = ht->buckets[i];

        while (list != NULL) {
            assert(num < ht->totalElems);
            result[num++] = strdup(list->key);
            list = list->next;
        }
    }

    assert(num == ht->totalElems);
    result[num] = NULL;
    return result;
}


void
deleteAllKeys(char **keys)      // IN
{
    free(keys);
}


void
deleteHashTable(hashtable *ht)      // IN
{
    char **keys = getAllKeys(ht);
    char **it;

    for (it = keys; *it != NULL; it++) {
        BOOL result = removeEntry(ht, *it);
        assert(result);
    }

    free(ht->buckets);
    free(ht);

    deleteAllKeys(keys);
}

// Trivial unit test
#if 1
int main()
{
    struct {
        char *key;
        int val;
    } items[] = {   
                    {"dog", 1232},
                    {"cat", 423435},
                    {"pig", 97432},
                    {"rat", 909787},
                };

    hashtable *ht = createHashtable(2);
    assert (ht);
    int i;
    void *dummy;
    int len = sizeof items / sizeof items[0]; 
    for (i = 0; i < len; i++) {
        printf("putting item: %s\n", items[i].key);
        assert(
            put(ht, items[i].key, (void *)items[i].val, &dummy) == 0
        );
    }

    assert(
        put(ht, "dog", (void *)934322, &dummy) == 1        
    );
    assert((int)dummy == 1232);

    int val;
    assert(
        get(ht, items[0].key, (void **)&val)
    );
    printf("key: %s, val: %d\n", items[0].key, val); 

    assert(
        !get(ht, "random", (void **)&val)
    );

    printf("num elems: %zu\n", getNumElems(ht));
    deleteHashTable(ht);

    return 0;
}

#endif // test code

