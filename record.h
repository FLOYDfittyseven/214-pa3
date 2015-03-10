#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

typedef char * string;

typedef struct recordNode {

    string filename;
    int count;
    struct recordNode  * next;

} recordNode;

typedef struct tokenNode{
	
    string word;
    recordNode *filelist;
    struct tokenNode *next; // A list of records
    
} tokenNode;

/* recordCompare compares two records. It compares them first by
 * count, then by filename if the counts are equal. It returns
 * -1 if arg1 goes before arg2 in the list, 1 if arg2 goes
 * before arg1 in the list, and 0 if they are identical.
 */
int recordCompare (recordNode *arg1, recordNode *arg2);


/*
 * DestroyRecordList takes a pointer to the head of a record list
 * and destroys the entire list, freeing all dynamically allocated memory.
 */
void DestroyRecordList(recordNode **head);


/*
 * DestroyTokenList takes a pointer to the head of a token list
 * and destroys the entire list, freeing all dynamically allocated memory.
 */
void DestroyTokenList(tokenNode **head);

/*
 * CreateRecord creates a new recordNode object. The caller must provide
 * the name of the file and a double pointer to its place in memory.
 */
void CreateRecord(recordNode **rec, string filename);


/*
 * CreateToken creates a new tokenNode object. The caller must provide
 * the word and filename and a double pointer to its place in memory.
 */
void CreateToken(tokenNode **tok, string word, string filename);


/*
 * InsertRecord inserts the given record into the list, maintaining order.
 * 
 * If the function succeeds, it returns 1. Otherwise, it returns 0.
 */
int InsertRecord(tokenNode **token, recordNode **record);


/*
 * UpdateRecord searches a record list for the given filename. If the
 * filename is found, its count is incremented and the record is reinserted
 * into the list to maintain order. Otherwise, a new recordNode
 * object is created and inserted into the list, maintaining order.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 */

int UpdateRecord(tokenNode **token, string filename);


/*
 * UpdateToken searches a token list for the given word. If the
 * word is found, filename's record is updated. Otherwise, a new tokenNode
 * object is created and inserted into the list, maintaining alphabetic
 * order.
 *
 * If the function succeeds, it returns 1, otherwise, it returns 0.
 */

int UpdateToken(tokenNode **head, string word, string filename);

#endif // RECORD_H_INCLUDED


