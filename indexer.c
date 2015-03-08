#include "record.h"
#include <string.h>

int recordCompare(struct record *arg1, struct record *arg2)
{
  if(arg1.tCount > arg2.tCount)
  {
    return 1;
  }
  else if (arg1.tCount < arg2.tCount)
  {
    return -1;
  }
  else
  {
    return 0;
  }
  
  int x = strcmp(arg1.filename, arg2.filename);
  
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
