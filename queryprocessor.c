#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queryprocessor.h"
#include "movieIndex.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"



SearchResultIter CreateSearchResultIter(MovieSet set) {
  SearchResultIter iter = (SearchResultIter)malloc(sizeof(struct searchResultIter)); 

  // Initialize docIter
  iter->docIter = CreateHashtableIterator((Hashtable)set->docIndex);

  if (iter->docIter == NULL ||
      (HTIteratorHasMore(iter->docIter) == 0)) {
    printf("Couldn't create an iterator; or iterator was empty (no docs)\n"); 
  }
  // Initialize offsetIter
  HTKeyValue *kvp = HTIteratorNext(iter->docIter); 
  // key is docid
  iter->curDocId = kvp->key;
  // value is offset list
  iter->offsetIter = CreateLLIter((LinkedList*)kvp->value); 
			
  return iter; 
}

void DestroySearchResultIter(SearchResultIter iter) {
  // Destroy LLIter
  if (iter->offsetIter != NULL) {
    DestroyLLIter(iter->offsetIter);
  }

  // Destroy docIter
  DestroyHashtableIterator(iter->docIter);

  free(iter); 
}



SearchResultIter FindMovies(Index index, char *term) {
  MovieSet set = GetMovieSet(index, term);
  if (set == NULL) {
    return NULL;
  }
  printf("Getting docs for movieset term: \"%s\"\n", set->desc); 
  SearchResultIter iter = CreateSearchResultIter(set); 
  return iter; 
}


// Return what for success, what for error? 
int GetNextSearchResult(SearchResultIter iter, SearchResult output) {
  // No more offsets for this doc
  if (HasNext(iter->offsetIter) == 0) {
    // destroy LLIter, get next docid, create new offsetIter. 
    DestroyLLIter(iter->offsetIter); 

    // Get next 
    if (HTIteratorHasMore(iter->docIter)) {
      HTKeyValue *kvp = HTIteratorNext(iter->docIter); 
      // key is docid
      iter->curDocId = kvp->key;
      // value is offset list
      iter->offsetIter = CreateLLIter((LinkedList*)kvp->value); 
    }
  }

  void *payload = Next(iter->offsetIter);
  int rowId = *((int*)payload); 
  output->docId = iter->curDocId;
  output->rowId = rowId; 
  return 0;  // we're at the end
}

// Return 0 if no more
int SearchResultIterHasMore(SearchResultIter iter) {
  if (HasNext(iter->offsetIter) == 0) {
    // destroy LLIter, get next docid, create new offsetIter. 
    DestroyLLIter(iter->offsetIter);
    iter->offsetIter = NULL;

    // Get next 
    if (HTIteratorHasMore(iter->docIter) != 0) {
      HTKeyValue *kvp = HTIteratorNext(iter->docIter); 
      // key is docid
      iter->curDocId = kvp->key;
      // value is offset list
      iter->offsetIter = CreateLLIter((LinkedList*)kvp->value); 
			
    } else {
      return 0; 
    }
  } 

  return 1; 
}





