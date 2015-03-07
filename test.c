#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	char *array[50];
	TokenizerT *t;
	
	fp = fopen("example.txt", "r");
	if(!fp)
	{
		exit(EXIT_FAILURE);
	}
	t = TKCreate(fp);
	
	int i;
	for(i=0; i<50; i++)
	{
		array[i] = NULL;
	}
	
	int z = 0;
	char * tmp;
	tmp = TKGetNextToken(t);
	while(tmp && z<50)
	{
		array[z] = tmp;
		printf("Token %d is %s.\n", z+1, tmp);
		z++;
		tmp = TKGetNextToken(t);
	}
	
	int j = 0;
	while(array[j])
	{
		free(array[j]);
		j++;
	}
	
	TKDestroy(t);
	int x = fclose(fp);
	
	if(x)
	{
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
