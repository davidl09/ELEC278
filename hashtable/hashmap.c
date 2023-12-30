//
// Created by davidl09 on 12/9/23.
//

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "hashmap.h"

//private type defs

struct item {
    keyType key;
    itemType value;
    bool occupied;
};

bool isprime(uint32_t n){
    for(int i=2;i*i<=n;i++){
        if(n%i==0){
            return false;
        }
    }
    return true;
}


uint32_t nextPrime(uint32_t num) {
    while (!isprime(++num));
    return num;
}

item *hashTable_find_internal__(hashTable *table, keyType key) {
    if (!table) return NULL;
    uint32_t index = table->hashFunc(key, table->capacity);

    while (index < table->capacity && table->data[index].occupied && !table->cmpFunc(key, table->data[index].key)) {
        ++index;
    }

    if (table->data[index].occupied && table->cmpFunc(key, table->data[index].key)) {
        return &table->data[index];
    }
    else return NULL;
}

hashTable make_hashTable(uint32_t capacity, hashFunction hashFunc, keyCmp cmpFunc, destructFunc destructor) {
    hashTable temp;
    temp.capacity = nextPrime(capacity);
    temp.hashFunc = hashFunc;
    temp.cmpFunc = cmpFunc;
    temp.destructItem = destructor;
    temp.data = calloc(temp.capacity, sizeof (item));
    return temp;
}

itemType *hashTable_find(hashTable *table, keyType key) {
    item *temp = hashTable_find_internal__(table, key);
    if (!temp) return NULL;
    else return &(temp->value);
}

void hashTable_grow(hashTable *table) {
    hashTable newTable = make_hashTable(2 * table->capacity, table->hashFunc, table->cmpFunc, table->destructItem);
    for (item *i = table->data; i < table->data + table->capacity; ++i) {
        hashTable_insert(&newTable, i->key, i->value);
    }

    free(table->data);
    *table = newTable;
}

void hashTable_insert(hashTable *table, keyType key, itemType value) {
    if (!table) return;
    if (table->capacity == 0 || table->data == NULL) {
        table->capacity = 10;
        hashTable_grow(table);
    }
    item *found = hashTable_find_internal__(table, key);
    uint32_t index = (found ? found - table->data : table->hashFunc(key, table->capacity));

    while (
            index < table->capacity
            && table->data[index].occupied
            && !table->cmpFunc(key, table->data[index].key)
            ) { //find first empty slot - if item with identical key found, overwrite
        ++index;
    }

    if (index == table->capacity) {
        hashTable_grow(table);
        hashTable_insert(table, key, value);
        return;
    }

    if (table->data[index].occupied) {
        table->destructItem(&table->data[index].value);
    }

    table->data[index] = (item){.occupied = true, .key = key, .value = value};
}

void hashTable_remove(hashTable *table, keyType key) {
    item *it = hashTable_find_internal__(table, key);
    if (!it) return;
    table->destructItem(&it->value);

    uint32_t index = it - table->data;
    memset(it, 0, sizeof (*it));

    for (; index < table->capacity; ++index) { //rehash and reinsert all items above removed item
        if (!table->data[index].occupied) continue;
        item temp = table->data[index];
        memset(table->data + index, 0, sizeof (temp)); //no need to destruct since item is being moved
        hashTable_insert(table, temp.key, temp.value);
    }
}

void free_hashTable(hashTable *table) {
    for (item *data = table->data; data < table->data + table->capacity; ++data) {
        table->destructItem(&data->value);
    }
    free(table->data);
    table->data = NULL;
    table->capacity = 0;
}