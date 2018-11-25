#include <stdlib.h>

#include "docidmap.h"
#include "htll/Hashtable.h"


void DestroyString(void *val) {
    free(val); 
}

DocIdMap CreateDocIdMap() {
  DocIdMap docs = (DocIdMap)CreateHashtable(64);
  return docs; 
}

void DestroyDocIdMap(DocIdMap map) {
  DestroyHashtable(map, &DestroyString); 
}

void PutFileInMap(char *filename, DocIdMap map) {
  HTKeyValue kvp; 
  kvp.key = NumElemsInHashtable(map) + 1; 
  kvp.value = filename;
  PutInHashtable(map, kvp);
}

DocIdIter CreateDocIdIterator(DocIdMap map) {
  HTIter iter = CreateHashtableIterator(map);
  return iter; 
}


void DestroyDocIdIterator(DocIdIter iter) {
  DestroyHashtableIterator(iter); 
}

char *GetFileFromId(DocIdMap docs, int docId) {
  HTKeyValue kvp; 
  int result = LookupInHashtable(docs, docId, &kvp);
  if (result == 0) 
    return (char*)kvp.value; 
  else return NULL; 

}
