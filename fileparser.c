#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

#include "movieIndex.h"
#include "fileparser.h"
#include "movie.h"
#include "docidmap.h"

#define NTHREADS 5
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
//=======================
// To minimize the number of files we have, I'm 
// putting the private function prototypes for 
// the fileparser here. 

void IndexTheFile(char *file, uint64_t docId, Index index);
void IndexTheFile_MT(char *file, uint64_t docId, Index index);
void IndexTheFile_MT_Helper(void *argp);
//=======================

struct index_args {
  char *file;
  uint64_t docId;
  Index index;
};

int ParseTheFiles(DocIdMap docs, Index index) { 
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  // For all the files in the docIdMap
  // Index the file. 
  // YOUR CODE GOES HERE
  ParseTheFiles_MT(docs, index); 
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("\n\nTook %f microseconds to parse %d files. \n\n", cpu_time_used, NumElemsInHashtable(docs));
  return 0; 
}


void IndexTheFile(char *file, uint64_t docId, Index index) {
  FILE *cfPtr;
  // Open a file
  // Use CreateMovieFromRow to create a movie
  // Put it in the index

  // YOUR CODE GOES HERE
  cfPtr = fopen(file, "r");
  char buffer[1024];
  int rowId = 0;
  while(fgets(buffer, sizeof(buffer), cfPtr)) {
    Movie* movie = CreateMovieFromRow(buffer);
    AddMovieToIndex(index, movie, docId, rowId); 
    DestroyMovie(movie);
    rowId++;
  }
  fclose(cfPtr);
}


int ParseTheFiles_MT(DocIdMap docs, Index index) {
	// Create the iterator
	// Create the threads
	// Spawn the threads to work on the function IndexTheFile_MT
  pthread_t tid[NTHREADS];
  DocIdIter iter = CreateDocIdIterator(docs);
  int count = 0;
  bool joined = false;
  while (HTIteratorHasMore(iter) != 0) {  
    HTKeyValue* iterVal = HTIteratorNext(iter);
    char *file = GetFileFromId(docs, iterVal->key);
    struct index_args *args = malloc(sizeof *args);
    args->file = file;
    args->docId = iterVal->key;
    args->index = index;

    pthread_create(&tid[count], NULL, IndexTheFile_MT_Helper, args);
    if (count < 4) {
      joined = false;
      count++;
    } else {
      for (int i = 0; i < count; i++) {
        pthread_join(tid[i], NULL);
      }
      joined = true;
      count = 0;
    }
    if (joined == false) {
      for (int i = 0; i < count; i++) {
        pthread_join(tid[i], NULL);
      }
    }
  }
  DestroyDocIdIterator(iter);
  return 0; 
}

void IndexTheFile_MT_Helper(void *argp) {
  struct index_args *a = argp;
  char *file = a->file;
  uint64_t docId = a->docId;
  Index index = a->index;
  IndexTheFile_MT(file, docId, index);
  free(a);
}

void IndexTheFile_MT(char *file, uint64_t docId, Index index) {

	// Lock the iterator
	// Get the filename, unlock the iterator
	// Read the file
	// Create movie from row
	// Lock the index
	// Add movie to index
	// Unlock the index

  FILE *cfPtr;
  pthread_mutex_lock(&m);
  cfPtr = fopen(file, "r");
  pthread_mutex_unlock(&m);

  char buffer[1024];
  int rowId = 0;
  while(fgets(buffer, sizeof(buffer), cfPtr)) {
    Movie* movie = CreateMovieFromRow(buffer);
    pthread_mutex_lock(&m);
    //printf("adding %s to index\n", movie->title);
    AddMovieToIndex(index, movie, docId, rowId); 
    pthread_mutex_unlock(&m);
    DestroyMovie(movie);
    rowId++;
  }
  fclose(cfPtr);
}



