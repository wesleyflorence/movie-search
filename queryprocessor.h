#include "movieIndex.h"

//===========================
// 
// Given a search term, finds results
// in the index and provides an iterator
// to go through all of the results. 
//
//===========================

typedef struct searchResult {
	uint64_t docId; 
  	int rowId;
} *SearchResult;


typedef struct searchResultIter {
	int curDocId; 
	HTIter docIter; 
	LLIter *offsetIter; 
} *SearchResultIter; 


/**
 *  Destroys and frees the iter.
 */
void DestroySearchResultIter(SearchResultIter iter);

/**
 * Puts the next result in the provided output 
 * argument. 
 */ 
int GetNextSearchResult(SearchResultIter iter, SearchResult output);

/** 
 *  Returns whether there are more search results to iterate through. 
 */ 
int SearchResultIterHasMore(SearchResultIter iter); 

/**
 * Given the index and a term, creates a new search 
 * result iterator that provides a docId/rowId for 
 * each entry in the MovieSet. 
 */
SearchResultIter FindMovies(Index index, char *term);
