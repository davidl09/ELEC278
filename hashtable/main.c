#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"


bool cmpStr(keyType a, keyType b) {
    return !strcmp(a, b);
}

uint32_t hashStr(char *str, uint32_t maxHash) {
    unsigned long hash = 5381;
    int c;

    while ((c = *(unsigned char *)str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % maxHash;
}

void intDestruct(int *obj) {}

int main() {
    hashTable t = make_hashTable(100, hashStr, cmpStr, intDestruct);
    itemType testItem = 100;
    hashTable_insert(&t, "key", testItem);
    assert(*hashTable_find(&t, "key") == testItem);

    // Test insert() and find() for multiple items
    itemType item1 = 10;
    itemType item2 = 20;
    itemType item3 = 30;
    hashTable_insert(&t, "key1", item1);
    hashTable_insert(&t, "key2", item2);
    hashTable_insert(&t, "key3", item3);
    assert(*hashTable_find(&t, "key1") == item1);
    assert(*hashTable_find(&t, "key2") == item2);
    assert(*hashTable_find(&t, "key3") == item3);

    // Test remove() and find() after removal
    hashTable_remove(&t, "key2");
    assert(hashTable_find(&t, "key2") == NULL);
    free_hashTable(&t);

}
