/*
 * tokenizer.c
 */
#include "tokenizer.h"

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <ctype.h>

#define MAXTOKENSIZE 100

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a file pointer).
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 */

TokenizerT *TKCreate( FILE *fp ) {

  /* Allocate memory for the TokenizerT this function is creating
   */
  TokenizerT * retTK;
  retTK = malloc(sizeof(TokenizerT));
  if(retTK == NULL || fp == NULL)
    {
      return NULL;
    }
  
  retTK->stream = fp;

  return retTK;
}

/*
 * TKDestroy destroys a TokenizerT object.
 */

void TKDestroy( TokenizerT * tk ) {

  /* Free the TokenizerT
   */
  free(tk);

  return;
}

/*
 * TKGetNextToken returns the next token from the file as a
 * character string.  Space for the returned token should be dynamically
 * allocated.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
	
	char tmpToken[MAXTOKENSIZE+1];
	memset(tmpToken, '\0', (MAXTOKENSIZE+1));
	int count;

/*
  char * first, * last, * tmp;
  first = tk->untokenized;
  last = first;
  */
  
  int x;
  x = tolower(fgetc(tk->stream));
  
  if(feof(tk->stream))
  {
	  return 0;
  }
  else if (!isalpha(x))
  {
	  while(!isalpha(x))
	  {
		  if(feof(tk->stream))
		  {
			  return 0;
		  }
		  
		  x = tolower(fgetc(tk->stream));
	  }
  }
  
  /*
   * Found first character of token
   */
   count = 1;
   tmpToken[count-1] = (char)x;
   
   x = tolower(fgetc(tk->stream));
   
   while(isalnum(x) && count<MAXTOKENSIZE)
   {
	   count++;
	   tmpToken[count-1] = (char)x;
	   
	   x = tolower(fgetc(tk->stream));
   }
   
   char *retToken = malloc((count+1) * sizeof(char));
   memcpy(retToken, tmpToken, count);
   retToken[count] = '\0';
 
  return retToken;
}


