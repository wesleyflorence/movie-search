#ifndef DOCSET_H
#define DOCSET_H

#include "htll/Hashtable.h"

//============================
//
// A "set of documents", which
// happen to be movies for this
// application.
//
// A movieSet refers to a set of documents
// that contain a certain word, which is
// specified in movieSet->desc.
//
// 
//
//============================


typedef struct movieSet {
  char *desc; 
  Hashtable docIndex; 
} *MovieSet;


// Given a set of movies, adds a given doc Id and rowId to it.
// The file that maps to the given docId contains the word
// for this movieset in the specified rowId. 
int AddMovieToSet(MovieSet set, uint64_t docId, int rowId); 

// Destroys and frees a MovieSet. 
void DestroyMovieSet(void *val); 

// Given a movieset, determines if the given doc
// is in this set (which means it contains the specified
// word for this MovieSet). 
int MovieSetContainsDoc(MovieSet set, uint64_t docId);

// Malloc's and creates a MovieSet
// for the provided word. 
MovieSet CreateMovieSet(char *desc);


// Frees and destroys the offset list for this movieset. 
void DestroyOffsetList(void *val);

// Frees and destroys this movieSet. 
void DestroyMovieSet(void *val);

#endif
