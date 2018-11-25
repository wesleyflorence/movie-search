#ifndef FILECRAWLER_H
#define FILECRAWLER_H


#include "docidmap.h"
#include "htll/LinkedList.h"


typedef LinkedList *FileSet; 


void CrawlFilesToMap(char *dir, DocIdMap map);


#endif // FILECRAWLER_H