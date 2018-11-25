#ifndef DOCIDMAP_H
#define DOCIDMAP_H

#include "htll/Hashtable.h"

/**
 *  A DocIdIter is a HTIter with some wrapping functions 
 *  that takes care of some the details associated with 
 *  creating/destroying the HTIter. 
 *
 *  All the functions that apply to an HTIter apply to a 
 *  DocIdIter, but you are encouraged to use the provided 
 *  wrapper functions when appropriate. 
 */
typedef HTIter DocIdIter;

/**
 * A DocIdMap is just a Hashtable, but the typedef 
 * allows us to write some wrapper functions that help 
 * simplify the put/get/create/destroy functions. 
 *
 * The keys are uint_64s, and the values are char*s that 
 * are filenames with paths. 
 *
 * All functions available for a Hashtable work on a 
 * DocIdMap, but you are encouraged to use the wrapper 
 * functions when appropriate. 
 */
typedef Hashtable DocIdMap;

/**
 *  Creates and returns a pointer to a DocIdMap. 
 *
 * 
 *
 */
DocIdMap CreateDocIdMap();

/**
 * Wrapper to destroy DocIdMap. 
 * 
 * Destroys and frees all data in the docidmap. 
 */
void DestroyDocIdMap(DocIdMap map); 

/**
 * Given a DocIdmap and a filename, puts the filename 
 * in the map, assigning it a unique ID. 
 * 
 * The string is copied and put into the DocIdMap, so after 
 * calling this the caller can free the string if necessary. 
 *
 */
void PutFileInMap(char *filename, DocIdMap map);

/**
 *  Given an DocIdMap, creates and returns an iterator 
 *  that provides a series of HTKeyValue structs (key-value pairs)
 *  where the key is a uint_64 and the value is a char*. 
 *
 *  A DocIdIter is an HTIterator, so all HTIterator functions apply. 
 */
DocIdIter CreateDocIdIterator(DocIdMap map);

void DestroyString(void *val); 

/**
 * Given a DocIdIter, destroys it and frees all the values (filenames as char*s)
 * 
 */ 
void DestroyDocIdIterator(DocIdIter iter); 

/**
 *  Given a DocIdMap and a docId, returns a char* that 
 *  is the corresponding docId. 
 *
 */
char *GetFileFromId(DocIdMap docs, int docId);


#endif
