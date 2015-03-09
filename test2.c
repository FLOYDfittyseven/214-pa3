#include "record.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	
	tokenNode *head = CreateToken("bottle", "baby.txt");
	
	head = UpdateToken(head, "bottle", "baby.txt");
	head = UpdateToken(head, "diaper", "baby.txt");
	head = UpdateToken(head, "diaper", "senior.txt");
	head = UpdateToken(head, "angel", "baby.txt");
	head = UpdateToken(head, "bottle", "almond.txt");
	head = UpdateToken(head, "bottle", "ace.txt");
	head = UpdateToken(head, "bottle", "ace.txt");
	head = UpdateToken(head, "bottle", "ace.txt");
	head = UpdateToken(head, "bottle", "ace.txt");
	head = UpdateToken(head, "bottle", "ace.txt");
	head = UpdateToken(head, "bottle", "ace.txt");
	
	
	tokenNode *tPtr = head;
	recordNode *recPtr;
	
	while(tPtr)
	{
		recPtr = tPtr->filelist;
		printf("Token: %s\n", tPtr->word);
		
		while(recPtr)
		{
			printf("Record: %s - %d\n\n", recPtr->filename, recPtr->count);
			recPtr = recPtr->next;
		}
		
		tPtr = tPtr->next;
	}
	
	exit(EXIT_SUCCESS);
}
