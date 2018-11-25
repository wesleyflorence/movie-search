
#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "movieIndex.h"
#include "docidmap.h"

//===============================
//
//  A fileparser parses rows of a file
//  and adds them to the indexer. 
//
//===============================

// Given a DocIdMap, go through all the files
// in the map and index them. 
int ParseTheFiles(DocIdMap docs, Index index); 

// Given a DocIdMap, go through all the files
// and index them. This one does it using multiple
// threads. 
int ParseTheFiles_MT(DocIdMap docs, Index index); 


#endif
