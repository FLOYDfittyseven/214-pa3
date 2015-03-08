/*
 * Generic map implementation. This class is thread-safe.
 * free() must be invoked when only one thread has access to the hashmap.
 */
#include < stdlib.h >
#include < stdio.h >
#include "record.h"

#define sizeMap 1024

// We need to keep keys and values
typedef struct hashElement{
	int key;
	record element;
} hashElement;

// A hashmap has some maximum size and current size,
// as well as the data to hold.
typedef struct hashmap{
	int size;
	hashElement *data;
} hashmap;

hashmap * hashCreate(){
    nashmap* map = (hashmap*) malloc(sizeof(hashmap));
	if(!map) return NULL;

	map->data = (hashmap*) malloc( sizeof(hashmap) * sizeMap);
	if(!map->data) return NULL;
    map->size = sizeMap

	return map;
}

