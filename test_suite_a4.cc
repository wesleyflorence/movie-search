// CS 5007, Northeastern University, Seattle
// Summer 2017
//
// Assignment 4: Multithreaded File Parser
#include "gtest/gtest.h"

extern "C" {
#include "htll/Hashtable.h"
#include "htll/LinkedList.h"
#include "docidmap.h"
#include "filecrawler.h"
#include "fileparser.h"
#include "movieIndex.h"
#include "movie.h"
#include <string.h>
}

TEST(DocIdMap, Full) {
  // Create DocIdMap
  DocIdMap docs = CreateDocIdMap();
  
  EXPECT_TRUE(docs != NULL);
  
  // Some files
  // Not ideal; opportunity for improvement next time around. 
  char *f1 = (char*)(malloc(sizeof(char)*10)); 
  strcpy(f1, "file1"); 
  PutFileInMap(f1, docs);
  EXPECT_EQ(NumElemsInHashtable(docs), 1);
  
  HTKeyValue kvp;
  LookupInHashtable(docs, 1, &kvp);
  EXPECT_EQ(strcmp((const char*)kvp.value, "file1"), 0); 
  
  char *f2 = (char*)(malloc(sizeof(char)*10));
  strcpy(f2, "file2");
  PutFileInMap(f2, docs);
  EXPECT_EQ(NumElemsInHashtable(docs), 2); 
  
  HTKeyValue kvp2;
  LookupInHashtable(docs, 2, &kvp2);
  EXPECT_EQ(strcmp((const char*)kvp2.value, "file2"), 0);

  char *out = GetFileFromId(docs, 1);
  EXPECT_EQ(strcmp(out, "file1"), 0);

  DocIdIter docIter = CreateDocIdIterator(docs);

  int numItems = 0; 
  while (HTIteratorHasMore(docIter) != 0) {
    HTKeyValue *kvp3 = HTIteratorNext(docIter);
    EXPECT_TRUE(numItems < 2); 
    numItems++;
  }

  EXPECT_EQ(numItems, 2);

  DestroyDocIdIterator(docIter); 
  
  DestroyDocIdMap(docs); 
}

TEST(FileCrawler, Create) {
    // Create a DocIdMap
  DocIdMap docs = CreateDocIdMap(); 
  // Choose a directory
  CrawlFilesToMap("data_tiny/", docs); 
  
  EXPECT_EQ(NumElemsInHashtable(docs), 10);

  int ids[11] = {0};
  
  HTIter iter = CreateHashtableIterator(docs); 
  
  while (HTIteratorHasMore(iter) != 0) {
    HTKeyValue *kv = HTIteratorNext(iter); 
    //   printf("docId: %d\tfilename: %s\n", kv->key, kv->value);
    ids[kv->key]++; 
  }

  // Should be a unique id for each file. 
  for (int i=1; i<=10; i++) {
    EXPECT_EQ(ids[i], 1); 
  }
  //DestroyHashtableIterator added by Wes to fixed valgrind leak:
  DestroyHashtableIterator(iter);
  DestroyDocIdMap(docs); 
}

TEST(MovieStruct, Full) {
  // Again, the weirdness to work with strings as not-constants.
  
  char row[100];
  strcpy(row,"1|movie|Sleepless in Seattle|Sleepless in Seattle|0|1984|\\N|120|Comedy,Romance"); 
  Movie *movie = CreateMovieFromRow(row);
  
  EXPECT_EQ(strcmp(movie->id, "1"), 0);
  EXPECT_EQ(strcmp(movie->type, "movie"), 0); 
  EXPECT_EQ(strcmp(movie->title, "Sleepless in Seattle"), 0);
  EXPECT_EQ(movie->isAdult, 0);
  EXPECT_EQ(movie->year, 1984);
  EXPECT_EQ(movie->runtime, 120);
  EXPECT_EQ(strcmp(movie->genres, "Comedy,Romance"), 0);

  DestroyMovie(movie); 
  
}

TEST(MovieIndex, Full) {
  // Create a movie index
  Index ind = CreateIndex();
  
  // Add some movies to the index
  Movie *m1 = CreateMovie();
  char title[100];
  strcpy(title, "Sleepless in Seattle");
  
  m1->title = title; 
  AddMovieToIndex(ind, m1, 1, 19);
  
  // Check the indices
  MovieSet set = GetMovieSet(ind, "seattle");
  EXPECT_EQ(strcmp(set->desc, "seattle"), 0);
  EXPECT_EQ(NumElemsInHashtable(set->docIndex), 1);
  HTKeyValue kvp;
  LookupInHashtable(set->docIndex, 1, &kvp);
  
  EXPECT_EQ(NumElementsLinkedList((LinkedList*)kvp.value), 1);
  EXPECT_EQ(NumElemsInHashtable(ind), 3); 
  
  DestroyMovie(m1);
  DestroyIndex(ind);
}


TEST(FileParser, Full) {
  // Given a docIdMap and an index,
  // the index should contain the info.
  DocIdMap docs = CreateDocIdMap();
  // Some files
  // Not ideal; opportunity for improvement next time around. 
  char *f1 = (char*)(malloc(sizeof(char)*30)); 
  strcpy(f1, "data_tiny/tinyaa"); 
  PutFileInMap(f1, docs);
  EXPECT_EQ(NumElemsInHashtable(docs), 1);

  Index ind = CreateIndex();

  ParseTheFiles(docs, ind);

  MovieSet set = GetMovieSet(ind, "coffee.");
  ASSERT_FALSE(set == NULL); 
  EXPECT_EQ(NumElemsInHashtable(set->docIndex), 1);

  set = GetMovieSet(ind, "elephant");
  EXPECT_EQ(NumElemsInHashtable(set->docIndex), 1);

  set = GetMovieSet(ind, "foobar"); 
  EXPECT_TRUE(set == NULL); 

  DestroyIndex(ind);
  DestroyDocIdMap(docs);
  // Not freeing f1 because DestroyDocIdMap does
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
