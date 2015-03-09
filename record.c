#include "record.h"
#include <string.h>
#include <stdlib.h>

/* recordCompare compares two records. It compares them first by
 * count, then by filename if the counts are equal. It returns
 * -1 if arg1 goes before arg2 in the list, 1 if arg2 goes
 * before arg1 in the list, and 0 if they are identical.
 */
int recordCompare(recordNode *arg1, recordNode *arg2)
{
  if(arg1->count > arg2->count)
  {
    return -1;
  }
  else if (arg1->count < arg2->count)
  {
    return 1;
  }
  
  int x = strcmp(arg1->filename, arg2->filename);
  
  if(x < 0)
  {
    return -1;
  }
  else if (x > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }

}

/*
 * DestroyRecordList takes a pointer to the head of a record list
 * and destroys the entire list, freeing all dynamically allocated memory.
 */
void DestroyRecordList(recordNode *head)
{
	if(!head)
	{
		return;
	}
	
	recordNode *ptr = head->next;
	
	while(ptr)
	{
		free(head);
		head = ptr;
		ptr = ptr->next;
	}
	
	free(head);
	
	return;	
}


/*
 * DestroyTokenList takes a pointer to the head of a token list
 * and destroys the entire list, freeing all dynamically allocated memory.
 */
void DestroyTokenList(tokenNode *head)
{
	if(!head)
	{
		return;
	}
	
	tokenNode *ptr = head->next;
	
	while(ptr)
	{
		DestroyRecordList(head->filelist);
		free(head);
		head = ptr;
		ptr = ptr->next;
	}
	
	DestroyRecordList(head->filelist);
	free(head);
	
	return;
}

/*
 * CreateRecord creates a new recordNode object. The caller must provide
 * the name of the file.
 * 
 * If the function succeeds, it returns a (non-NULL) recordNode object,
 * otherwise, it returns NULL.
 */
recordNode *CreateRecord(string filename)
{
	if(!filename)
	{
		return NULL;
	}
	
	recordNode *retRec = malloc(sizeof(recordNode));
	
	if(!retRec)
	{
		return NULL;
	}
	
	retRec->filename = filename;
	retRec->count = 1;
	retRec->next = NULL;
	
	return retRec;
}


/*
 * CreateToken creates a new tokenNode object. The caller must provide
 * the word and filename.
 * 
 * If the function succeeds, it returns a (non-NULL) tokenNode object,
 * otherwise, it returns NULL.
 */
tokenNode *CreateToken(string word, string filename)
{
	if(!word || !filename)
	{
		return NULL;
	}
	
	tokenNode *retToken = malloc(sizeof(tokenNode));
	
	if(!retToken)
	{
		return NULL;
	}
	
	retToken->word = word;
	retToken->filelist = CreateRecord(filename);
	retToken->next = NULL;
	
	return retToken;
}


/*
 * InsertRecord inserts the given record into the list, maintaining order.
 * 
 * If the function succeeds, it returns 1. Otherwise, it returns 0.
 */
int InsertRecord(tokenNode *token, recordNode *record)
{
	if(!token || !record)
	{
		return 0;
	}
	
	recordNode *compPtr = token->filelist;
	recordNode *newPrev = compPtr;
	int x;
	
	if(recordCompare(compPtr, record) > 0)
	{
		record->next = compPtr;
		token->filelist = record;
		return 1;
	}
	
	compPtr = compPtr->next;
	
	while(compPtr)
	{
		x = recordCompare(compPtr, record);
		
		if(x > 0)
		{
			break;
		}
		
		newPrev = compPtr;
		compPtr = compPtr->next;
	}
	
	record->next = compPtr;
	newPrev->next = record;
	
	return 1;
}


/*
 * UpdateRecord searches a record list for the given filename. If the
 * filename is found, its count is incremented and the record is reinserted
 * into the list to maintain order. Otherwise, a new recordNode
 * object is created and inserted into the list, maintaining order.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 */

int UpdateRecord(tokenNode *token, string filename)
{
	if(!token || !filename)
	{
		return 0;
	}
	
	int x, y, lowestNum;
	recordNode *ptr = token->filelist;
	recordNode *prev = ptr;
	recordNode *low = NULL;
	recordNode *low_prev = NULL;
	
	x = strcmp(ptr->filename, filename);
	ptr = ptr->next;
	
	/* Head is incremented
	 */
	if(x == 0)
	{
		prev->count++;
		return 1;
	}
	
	lowestNum = prev->count;
	low = prev;
		
	while(ptr)
	{
		x = strcmp(ptr->filename, filename);
		
		if(x == 0)
		{
			break;
		}
		/* Reached the first record with a lower count. Make pointer
		 * in case we have to create and compare a new record.
		 */
		else if(ptr->count < lowestNum)
		{
			low = ptr;
			low_prev = prev;
			lowestNum = ptr->count;
		}
	
		prev = ptr;
		ptr = ptr->next;
	}
	
	/* Found a match. Increment and reinsert into the list.
	 */
	if(ptr)
	{
		
		prev->next = ptr->next;
		ptr->count++;
		y = InsertRecord(token, ptr);
		
		return y;
	}
	/* Did not find a match. Create new record and use low to
	 * insert into the list.
	 */
	else
	{
		recordNode *newRecord = CreateRecord(filename);
		
		if(low_prev)
		{
			while(low)
			{
				if(recordCompare(low, newRecord) > 0)
				{
					break;
				}
				low_prev = low;
				low = low->next;
			}
			
			low_prev->next = newRecord;
			newRecord->next = low;
			return 1;
		}
		else
		{
			y = InsertRecord(token, newRecord);
			return y;
		}
	}
}


/*
 * UpdateToken searches a token list for the given word. If the
 * word is found, filename's record is updated. Otherwise, a new tokenNode
 * object is created and inserted into the list, maintaining alphabetic
 * order.
 *
 * If the function succeeds, it returns a non-NULL tokenNode pointer,
 * otherwise it returns NULL.
 */

tokenNode *UpdateToken(tokenNode *head, string word, string filename)
{
	if(!head || !word || !filename)
	{
		return NULL;
	}
	
	tokenNode *ptr = head;
	tokenNode *prev = head;
	tokenNode *newNode;
	int x, y;
	
	x = strcmp(ptr->word, word);
	
	/* Create new tokenNode object and make it the head
	 */
	if(x > 0)
	{
		newNode = CreateToken(word, filename);
		newNode->next = ptr;
		
		return newNode;
	}
	/* Head is a match
	 */
	else if(x == 0)
	{
		y = UpdateRecord(ptr, filename);
		
		return head;
	}
	
	ptr = ptr->next;
	
	while(ptr)
	{
		x = strcmp(ptr->word, word);
		
		if(x > 0)
		{
			break;
		}
		else if(x == 0)
		{
			y = UpdateRecord(ptr, filename);
			
			return head;
		}
		
		prev = ptr;
		ptr = ptr->next;
	}
	
	newNode = CreateToken(word, filename);
	newNode->next = ptr;
	prev->next = newNode;
	
	return head;
}

