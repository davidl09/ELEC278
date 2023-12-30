//
// Created by davidl09 on 12/9/23.
//

#ifndef HASHTABLE_HASHMAP_H
#define HASHTABLE_HASHMAP_H

#include <stdint.h>
#include <stdbool.h>

#define keyType char *
#define itemType int

typedef uint32_t(*hashFunction)(keyType key, uint32_t maxIndex);
//Hashes the given keyType and returns a value in the range [0, maxIndex)
typedef bool (*keyCmp)(keyType, keyType);
typedef void (*destructFunc)(itemType *);

typedef struct item item;

typedef struct hashTable {
    item *data;
    hashFunction hashFunc;
    keyCmp cmpFunc;
    destructFunc destructItem;
    uint32_t capacity;
} hashTable;

hashTable make_hashTable(uint32_t capacity, hashFunction hashFunc, keyCmp cmpFunc, destructFunc destructor);

itemType *hashTable_find(hashTable *table, keyType key);

void hashTable_insert(hashTable *table, keyType key, itemType item);

void hashTable_remove(hashTable *table, keyType key);

void free_hashTable(hashTable *table);

#endif //HASHTABLE_HASHMAP_H
