#include "sorted-list.h"
#include "record.h"
#include <string.h>

/* recordCompare compares two records. It compares them first by
 * count, then by filename if the counts are equal. It returns
 * -1 if arg2 goes before arg1 in the sorted-list, 1 if arg1 goes
 * before arg2 in the sorted-list, and 0 if they are identical.
 */
int recordCompare(void *arg1, void *arg2)
{
  if(arg1->count > arg2->count)
  {
    return 1;
  }
  else if (arg1->count < arg2->count)
  {
    return -1;
  }
  else
  {
    return 0;
  }
  
  int x = strcmp(arg1->filename, arg2->filename);
  
  if(x < 0)
  {
    return 1;
  }
  else if (x > 0)
  {
    return -1;
  }
  else
  {
    return 0;
  }

}


/*
 * tokenCompare compares two tokens. It returns -1 if arg2 goes
 * before arg1, 1 if arg1 goes before arg2, and 0 if they are identical.
 */
int tokenCompare(void *arg1, void *arg2)
{
	int x = strcmp(arg1->word, arg2->word);
	
	if(x < 0)
	{
		return 1;
	}
	else if(x > 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
