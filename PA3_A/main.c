#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tokenizer.c"
#include "record.h"

int main()
{
    printf("Hello world!\n");
    FILE *fp;
    int ch;
    fp=fopen("/Users/Anush/Documents/214-pa3/214-pa3/PA3_A/lorem.txt", "r");
    TokenizerT *tokenizer = TKCreate(fp);
    char *retToken;
    char str[15];


 /* int MAXSIZE = 500;
    char proclnk[500];
    char *filename= malloc(sizeof(char)*500);
    int fno;
    ssize_t r;
    // test.txt created earlier
//    fp = fopen("test.txt", "r");
    if (fp != NULL)
    {
        fno = fileno(fp);
        sprintf(proclnk, "/proc/self/fd/%d", fno);
        printf("%s %d\n",proclnk,fno);
        r = readlink(proclnk, filename, MAXSIZE);
        printf("%d\n", r);
        if (r < 0)
        {
            printf("failed to readlink\n");
            exit(1);
        }
        filename[r] = '\0';
        printf("fp -> fno -> filename: %p -> %d -> %s\n",
                fp, fno, filename);
    }

free (filename); */

record *one, *two, *three;
one = (record *)malloc(sizeof(record));
two = (record *)malloc(sizeof(record));
three = (record *)malloc(sizeof(record));
retToken = TKGetNextToken(tokenizer);
//printf("%s ",retToken);

 int count;
 one->fileList = (nodeRec *)malloc(sizeof(nodeRec));
 one->fileList->filename="first file";
 one->word=retToken;
 one->fileList->count=1;
// one->next= NULL;

retToken = TKGetNextToken(tokenizer);
two->fileList = (nodeRec *)malloc(sizeof(nodeRec));
 two->fileList->filename="first file";
 two->word=retToken;
 two->fileList->count=1;
 one->next = two;
retToken = TKGetNextToken(tokenizer);

three->fileList = (nodeRec *)malloc(sizeof(nodeRec));
 three->fileList->filename="first file";
 three->word=retToken;
 three->fileList->count=1;
 two->next = three;
 record * tmp = one;
 //printf("word %s count %d filename %s\n", tmp->word, tmp->fileList->count, three->fileList->filename);

 while(tmp->next != NULL){
    printf("word %s count %d  name %s\n", tmp->word, tmp->fileList->count, tmp->fileList->filename);
    tmp=tmp->next;
 }
 //printf("word %s count %d filename %s\n", tmp->word, tmp->fileList->count, three->fileList->filename);
    free(one);
  free(two);
   free(three);
    fclose(fp);
    return 0;
}
