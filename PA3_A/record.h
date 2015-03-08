#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED
typedef char * string;
/*typedef struct nodeRec {
    string filename;
    int count;
    struct nodeRec *next;
} nodeRec ;

typedef struct record{
    string word;
    nodeRec *fileList;
    struct record * next;
}record ;*/



typedef struct recordNode {

    string filename;
    int count;
    recordNode * next;


} recordNode;

typedef struct tokenNode{
    string word;
    recordNode *filelist;
    struct tokeNode *next; // A SortedList of records
} tokenNode;

/* recordCompare compares two records. It compares them first by
 * count, then by filename if the counts are equal. It returns
 * -1 if arg2 goes before arg1 in the sorted-list, 1 if arg1 goes
 * before arg2 in the sorted-list, and 0 if they are identical.
 */
//int recordCompare (struct recordNode *arg1, struct recordNode *arg2);


#endif // RECORD_H_INCLUDED
