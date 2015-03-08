#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED
typedef char * string;
typedef struct nodeRec {
    string filename;
    int count;
    struct nodeRec *next;
} nodeRec ;

typedef struct record{
    string word;
    nodeRec *fileList;
    struct record * next;
}record ;

#endif // RECORD_H_INCLUDED
