#include "tokenizer.h"
#include "record.h"
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <linux/limits.h>


/* FindFile searches the current directory for a file with the given name.
 * 
 * The function returns 1 if it finds the file, 0 if it doesn't, and -1 if 
 * there is an error.
 */
int FindFile (char *filename)
{
	DIR *curDir;
	struct dirent *entry;
	
	curDir = opendir(".");
	
	if(!curDir)
	{
		return -1;
	}
	
	while((entry = readdir(curDir)))
	{
		if(strcmp(filename, entry->d_name) == 0)
		{
			closedir(curDir);
			return 1;
		}
	}
	
	closedir(curDir);
	return 0;
}


/* GetType determines whether filename is a directory, a regular file,
 * or something else.
 * 
 * The function returns 1 if filename is a directory, 2 if it is a 
 * regular file, 0 if it is a different type of file, -1 if it is not
 * a file, and -2 if a match is not found.
 */
int GetType (char *filename)
{
	DIR *curDir;
	struct dirent *entry;
	struct stat s;
	
	curDir = opendir(".");
	
	if(!curDir)
	{
		return -1;
	}
	
	while((entry = readdir(curDir)))
	{
		if(strcmp(filename, entry->d_name) == 0)
		{
			if(stat(filename, &s) == 0)
			{
				/* Directory
				 */
				if(S_ISDIR(s.st_mode))
				{
					closedir(curDir);
					return 1;
				}
				/* Reg file
				 */
				else if(S_ISREG(s.st_mode))
				{
					closedir(curDir);
					return 2;
				}
				else
				{
					closedir(curDir);
					return 0;
				}
			}
			else
			{
				closedir(curDir);
				return -1;
			}
		}
	}
	
	closedir(curDir);
	return -2;
}


/* IndexFile indexes a single file into the given token List.
 */
void IndexFile (tokenNode **head, char *filename)
{
	FILE *fp;
	TokenizerT *t;
	string tmp;
	tokenNode *list = *head;
	int x;
	
	fp = fopen(filename, "r");
	
	if(!fp)
	{
		return;
	}
	
	t = TKCreate(fp);
	
	/* If token list is empty, create.
	 */
	if(!list)
	{
		tmp = TKGetNextToken(t);
		CreateToken(head, tmp, filename);
	}
	
	while((tmp = TKGetNextToken(t)))
	{
		UpdateToken(head, tmp, filename);
	}
	
	TKDestroy(t);
	x = fclose(fp);
	if(x)
	{
		fprintf(stderr, "Unable to close %s.\n", filename);
		exit(EXIT_FAILURE);
	}
	
	return;
}


/* IndexDir recursively indexes a directory.
 */
void IndexDir (tokenNode **head, char *dirName)
{
	DIR *directory;
	struct dirent *entry;
	char path[PATH_MAX];
	int pathLen;
	struct stat statbuf;
	
	directory = opendir(dirName);
	
	if(!directory)
	{
		fprintf(stderr, "Cannot open directory '%s'\n", dirName);
		return;
	}
	
	while((entry = readdir(directory)))
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			pathLen = snprintf(path, PATH_MAX, "%s/%s", dirName, entry->d_name);
			
			if(pathLen >= PATH_MAX)
			{
				fprintf(stderr, "Path is too long\n");
				closedir(directory);
				return;
			}
			
			stat(path, &statbuf);
			
			/* Check if directory
			 */
			if(S_ISDIR(statbuf.st_mode))
			{
				IndexDir(head, path);
			}
			/* Check if regular file
			 */
			else if(S_ISREG(statbuf.st_mode))
			{
				IndexFile(head, path);
			}
		}
	}
	
	closedir(directory);
	
	return;
}


/* WriteIndex writes an index "invertedIndex" to a file "stream".
 * 
 * If successful, the function returns 1, otherwise, it returns 0.
 */
int WriteIndex(FILE *stream, tokenNode *invertedIndex, char *target)
{
	
	if(!stream || !invertedIndex || !target)
	{
		return 0;
	}
	
	tokenNode *tPtr = invertedIndex;
	recordNode *recPtr;
	
	fprintf(stream, "{\"%s\" : [\n", target);
	
	while(tPtr->next)
	{
		recPtr = tPtr->filelist;
		fprintf(stream, "\t{\"%s\" : [\n", tPtr->word);
		
		while(recPtr->next)
		{
			fprintf(stream, "\t\t{\"%s\" : %d},\n", recPtr->filename, recPtr->count);
			recPtr = recPtr->next;
		}
		
		fprintf(stream, "\t\t{\"%s\" : %d}\n", recPtr->filename, recPtr->count);
		
		if(tPtr->next->next)
		{
			fprintf(stream, "\t]},\n");
		}
		tPtr = tPtr->next;
	}
	fprintf(stream, "\t]}\n");
	fprintf(stream, "]}");
	
	return 1;
}


int CreateIndex (char *writeName, char *target)
{
	int x;
	int ID;
	int z;
	tokenNode *i;
	char input;
	char newFile[100];
	FILE *outFile;
	
	x = FindFile(writeName);
	
	if(x == -1)
	{
		fprintf(stderr, "Unable to open current directory.\n");
		return 0;
	}
	
	/* File exists
	 */
	while(x)
	{
		printf("File already exits. Overwrite? (y or n)\n");
		scanf("%c", &input);
		
		while(input != 'y' && input != 'Y' && input != 'n' && input != 'N')
		{
			printf("'%c' is invalid input.\nOverwrite? (y or n)\n", input);
			scanf("%c", &input);
		}
		
		/* User does not want to overwrite
		 */
		if(input == 'n' || input == 'N')
		{
			printf("Do you want to choose a new file? (y or n)\n");
			scanf("%c", &input);
			
			while(input != 'y' && input != 'Y' && input != 'n' && input != 'N')
			{
				printf("'%c' is invalid input.\nChoose a new file? (y or n)\n", input);
				scanf("%c", &input);
			}
			
			/* User does not want to write to a new file
			 */
			if(input == 'n' || input == 'N')
			{
				return 0;
			}
			/* User does want to write to a new file
			 */
			else
			{
				printf("Input file name:\n");
				memset(newFile, '\0', 100);
				scanf("%s", newFile);
				
				x = FindFile(newFile);
			}
		}
		/* User does want to overwrite
		 */
		else
		{
			break;
		}
	}
	
	outFile = fopen(writeName, "w");
	
	/* Check if target is a directory or file
	 */
	 ID = GetType(target);
	 
	 if(ID == 0)
	 {
		 fprintf(stderr, "%s is incorrect type.\n", target);
		 return 0;
	 }
	 else if(ID == -1)
	 {
		fprintf(stderr, "%s is not a file.\n", target);
		return 0;
	 }
	 else if(ID == -2)
	 {
		 fprintf(stderr, "%s not found.\n", target);
		 return 0;
	 }
	 /* Indexing a directory
	  */
	 else if(ID == 1)
	 {
		 IndexDir(&i, target);
	 }
	 /* Indexing a single file
	  */
	 else
	 {
		 IndexFile(&i, target);
	 }
	 
	 if(!i)
	 {
		 fprintf(stderr, "Unable to index %s.\n", target);
		 return 0;
	 }
	 
	 z = WriteIndex(outFile, i, target);
	 
	 fclose(outFile);
	 DestroyTokenList(&i);
	 
	 return z;
}

int main ( int argc, char *argv[] )
{
	int y;
	
	if(argc != 3)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		exit(EXIT_FAILURE);
	}
	
	y = CreateIndex(argv[1], argv[2]);
	
	if(!y)
	{
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
