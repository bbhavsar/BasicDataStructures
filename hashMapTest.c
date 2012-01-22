#include <stdio.h>
#include <assert.h>

#include "hashMap.h"

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

    HashMap *hm = HashMap_create(2);
    assert (hm);
    int i;
    void *dummy;
    int len = sizeof items / sizeof items[0];
    for (i = 0; i < len; i++) {
        printf("putting item: %s\n", items[i].key);
        assert(
            HashMap_put(hm, items[i].key, (void *)items[i].val, &dummy) == 0
        );
    }

    assert(
        HashMap_put(hm, "dog", (void *)934322, &dummy) == 1
    );
    assert((int)dummy == 1232);

    int val;
    assert(
        HashMap_get(hm, items[0].key, (void **)&val)
    );
    printf("key: %s, val: %d\n", items[0].key, val);

    assert(
        !HashMap_get(hm, "random", (void **)&val)
    );

    printf("num elems: %zu\n", HashMap_getNumElems(hm));
    HashMap_delete(hm);

    return 0;
}

