#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "filecrawler.h"
#include "docidmap.h"
#include "LinkedList.h"





void CrawlFilesToMap(char *dir, DocIdMap map) {
  struct stat s; 

  struct dirent **namelist;
  int n;

  n = scandir(dir, &namelist, 0, alphasort);
  if (n < 0)
    perror("scandir");
  else {
    int i = 0;
    while (i < n) {
      if (namelist[i]->d_name[0] == '.') {
	free(namelist[i]);
	i++; 
	continue; 
      }
      char *directory;
      directory = (char*) malloc(((strlen(dir) +
				   (strlen(namelist[i]->d_name)) +
				   2)*sizeof(char)));
      strcpy(directory, dir);
      strcat(directory, namelist[i]->d_name);
      if (0 == stat(directory, &s)) {
	if (S_ISDIR(s.st_mode)) {
	  strcat(directory, "/"); 
	  CrawlFilesToMap(directory, map); 
	} else {
	  PutFileInMap(directory, map);
	}
      } else {
	printf("no stat; %s\n", directory);
      }
      free(namelist[i]);
      i++;
    }
  }
  free(namelist);
}
