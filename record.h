#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include "sorted-list.h"

typedef char * string;

typedef struct record {
	
string filename;
int count;

} record;

typedef struct token{
	
string word;
SortedListPtr fileList; // A SortedList of records

} token;

/* recordCompare compares two records. It compares them first by
 * count, then by filename if the counts are equal. It returns
 * -1 if arg2 goes before arg1 in the sorted-list, 1 if arg1 goes
 * before arg2 in the sorted-list, and 0 if they are identical.
 */
int recordCompare (struct recordNode *arg1, struct recordNode *arg2);


#endif // RECORD_H_INCLUDED
