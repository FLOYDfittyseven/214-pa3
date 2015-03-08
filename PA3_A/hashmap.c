/*
 * Generic map implementation. This class is thread-safe.
 * free() must be invoked when only one thread has access to the hashmap.
 */
#include <stdio.h>
#include <stdlib.h>
#include "record.h"

#define sizeMap 1024

// We need to keep keys and values
typedef struct hashElement{
	unsigned long key;
	tokenNode *record;
} hashElement;

// A hashmap has some maximum size and current size,
// as well as the data to hold.
typedef struct hashmap{
	int size;
	hashElement *table;
} hashmap;

 int addElement(hashmap *map, string token);
 hashmap * hashCreate();
 unsigned long hash(string str);

hashmap * hashCreate(){
    hashmap* map = (hashmap*) malloc(sizeof(hashmap));
	if(!map) {
            printf("failed to allocate memory for the map\n");
            return NULL;
	}

	map->table = (hashElement*) malloc( sizeof(hashElement) * sizeMap);
	if(!map->table) {
        printf("failed to create array\n");
        return NULL;
	}

    map->size = sizeMap;

	return map;
}

/*djb2 algorythm for hashing*/
unsigned long hash(string str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % sizeMap;
}


 int addElement(hashmap *map, string token)
{
//    hashElement *element;
    unsigned int hashval = hash(token);
    printf("function \n");
    printf("word %s hashvalue %d \n", token, hashval);
    /* Go to the correct list based on the hash value and see if str is
     * in the list.  If it is, return return a pointer to the list element.
     * If it isn't, the item isn't in the table, so return NULL.
     */
    tokenNode * list = (tokenNode*) malloc( sizeof(tokenNode *));
    list = map->table[hashval].record;
     if(list == NULL){ /*create a new node*/
        list = (tokenNode *)malloc(sizeof(tokenNode));
        list->filelist = (recordNode *)malloc(sizeof(recordNode));
        list-> word = token;
        if(list->filelist->count)
            list->filelist->count ++;
        else
            list->filelist->count=0;
        //free(list);
        return 1;
    }
    while(list->next!= NULL){
        list= list->next;
    }
    if(list->next == NULL){
        list = list->next;
        list = (tokenNode *)malloc(sizeof(tokenNode));
        list->filelist = (recordNode *)malloc(sizeof(recordNode));
        list-> word = token;
        if(list->filelist->count)
            list->filelist->count ++;
        else
            list->filelist->count=0;
        //free(list);
        return 1;
    }

    printf("error\n");
    //free(list);
    return 0;


}
