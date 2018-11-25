#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "movieIndex.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"
#include "movie.h"
#include "docset.h"

void toLower(char *str, int len) {
  for (int i=0; i<len; i++) {
    str[i] = tolower(str[i]); 
  }
}

int AddMovieToIndex(Index index, Movie *movie, uint64_t docId, int rowId) {
  // Put in the index
  HTKeyValue kvp;

  // TODO: Dunno if this is best.
  int numFields = 25; 
	
  char *token[numFields];
  char *rest = movie->title;

  int i=0;
  token[i] = strtok_r(rest, " ", &rest);
  while (token[i] != NULL) {
    toLower(token[i], strlen(token[i]));
    i++; 
    token[i] = strtok_r(rest, " ", &rest);
  }

  for (int j=0; j<i; j++) {
    // If this key is already in the hashtable, get the MovieSet. 
    // Otherwise, create a MovieSet and put it in. 
    int result = LookupInHashtable(index,
				   FNVHash64((unsigned char*)token[j],
					     (unsigned int)strlen(token[j])), &kvp);

    if(result < 0) {
      kvp.value = CreateMovieSet(token[j]); 
      kvp.key = FNVHash64((unsigned char*)token[j], strlen(token[j]));
      PutInHashtable(index, kvp);
    } 

    AddMovieToSet((MovieSet)kvp.value, docId, rowId);
  }

  return 0; 
}


MovieSet GetMovieSet(Index index, const char *term) {
  HTKeyValue kvp;
  char lower[strlen(term)+1];
  strcpy(lower, term); 
  toLower(lower, strlen(lower)); 
  int result = LookupInHashtable(index, FNVHash64((unsigned char*)lower,
						  (unsigned int)strlen(lower)), &kvp);
  if (result < 0) {
    printf("term couldn't be found: %s \n", term); 
    return NULL; 
  }
  return (MovieSet)kvp.value; 
}


int DestroyIndex(Index index) {
  DestroyHashtable(index, &DestroyMovieSet); 
  return 0;
}

Index CreateIndex() {
  return CreateHashtable(128);
}
