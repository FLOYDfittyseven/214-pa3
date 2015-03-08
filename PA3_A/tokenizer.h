#include <stdio.h>

/*
 * Tokenizer type. TokenizerT contains a FILE pointer called "stream".
 */

struct TokenizerT_ {

  FILE * stream;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a file pointer).
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 */

TokenizerT *TKCreate( FILE *fp );

/*
 * TKDestroy destroys a TokenizerT object.
 */

void TKDestroy( TokenizerT * tk );
 
/*
 * TKGetNextToken returns the next token from the file as a
 * character string.  Space for the returned token should NOT be dynamically
 * allocated.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk );

