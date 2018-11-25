#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "docset.h"
#include "Hashtable.h"

int AddMovieToSet(MovieSet set,  uint64_t docId, int rowId) {
  Hashtable docInd = set->docIndex;
  
  HTKeyValue kvp;
  int result = LookupInHashtable(docInd, docId, &kvp);
  
  //If the docId is in docInd, use it. 
  // Otherwise, create a new entry for this docId in docInd.
  if (result < 0) {
    kvp.key = docId;
    kvp.value = CreateLinkedList();
    PutInHashtable(docInd, kvp); 
  }
  
  // add rowId to the linked list.
  void *val = malloc(sizeof(int));

  if (val == NULL) {
    // Out of mem
    printf("Out of memory adding movie to set: %s\n", set->desc);
    return -1; 
  }
  
  *((int*)val) = rowId; 
  result = Insert((LinkedList*)kvp.value, val);

  return result; 
}

int MovieSetContainsDoc(MovieSet set, uint64_t docId) {
  HTKeyValue kvp;
  return LookupInHashtable(set->docIndex, docId, &kvp);
}

MovieSet CreateMovieSet(char *desc) {
  MovieSet set = (MovieSet)malloc(sizeof(struct movieSet));
  if (set == NULL) {
    // Out of memory
    printf("Couldn't malloc for movieSet %s\n", desc);
    return NULL; 
  }
  set->desc = (char*)malloc(strlen(desc) *  sizeof(char) + 1); 
  strcpy(set->desc, desc); 
  set->docIndex = CreateHashtable(16);
  return set; 
}

void DestroyOffsetList(void *val) {
  LinkedList *list = (LinkedList*)val;
  // Gotta free all the payloads first.
  LLIter *iter = CreateLLIter(list);
  while (HasNext(iter) != 0) {
    void *payload = Next(iter);
    free(payload); 
  }
  DestroyLLIter(iter); 
  DestroyLinkedList(list); 
}

void DestroyMovieSet(void *val) {
  MovieSet set = (MovieSet)val; 
  // Free desc
  free(set->desc); 
  // Free docIndex
  DestroyHashtable(set->docIndex, &DestroyOffsetList);
  // Free set
  free(set); 
}
