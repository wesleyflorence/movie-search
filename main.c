#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "htll/Hashtable.h"
#include "htll/LinkedList.h"

#include "docset.h"
#include "docidmap.h"
#include "fileparser.h"
#include "filecrawler.h"
#include "movieIndex.h"
#include "movie.h"
#include "queryprocessor.h"


//==================================================


DocIdMap docs;
Index docIndex; 

// To be in queryprocesser? 
int GetRowFromFile(char *file, long rowId) {
  FILE *fp;

  char buffer[1000];
  
   fp = fopen(file,"r");
  
   int i=0;
   while (i <= rowId) {
     fgets(buffer, 1000, fp);
     i++; 
   }
   // taking \n out of the row
   buffer[strlen(buffer)-1] = ' ';
   printf("%s (%s, row %ld)\n", buffer, file, rowId); 
   fclose(fp);
   return 0; 
}

void doPrep(char *dir) {
  printf("Crawling directory tree starting at: %s\n", dir)
    ; 
  // Create a DocIdMap
  docs = CreateHashtable(64); 
  CrawlFilesToMap(dir, docs);
  
  printf("Crawled %d files.\n", NumElemsInHashtable(docs)); 
  
  // Create the index
  docIndex = CreateIndex();
  
  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, docIndex); 
  printf("%d entries in the index.\n", NumElemsInHashtable(docIndex)); 
}

void runQuery(char *term) {
  SearchResultIter results = FindMovies(docIndex, term);
  if (results == NULL) {
    printf("No results for this term. Please try another.\n");
    return; 
  } else {
    SearchResult sr = (SearchResult)malloc(sizeof(*sr)); 
    while (SearchResultIterHasMore(results) != 0) {
      GetNextSearchResult(results, sr); 
      char *filename = GetFileFromId(docs, sr->docId); 
      GetRowFromFile(filename, sr->rowId); 
    }
    free(sr);
    DestroySearchResultIter(results);
  }
}

void runQueries() {
  char input[1000];

  while (1) {
    printf("\nEnter a term to search for, or q to quit: ");
    scanf("%s", input);
    
    if (strlen(input) == 1 &&
	(input[0] == 'q')) {
	printf("Thanks for playing! \n");
	return; 
      }
    
    printf("\n");
    
    runQuery(input);
  }
}


int main(int argc, char *argv[]){
  // Check arguments 
  if (argc != 2) {
    printf("Wrong number of arguments.\n");
    printf("usage: main <directory_to_crawl>\n");
    return 0; 
  }

  doPrep(argv[1]);

  runQueries(); 
  
  DestroyIndex(docIndex); 
  
  DestroyHashtable(docs, &DestroyString); 
}
