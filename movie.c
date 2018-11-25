#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "movie.h"

Movie* CreateMovie() {
	Movie *mov = (Movie*)malloc(sizeof(Movie));
	if (mov == NULL) {
	  printf("Couldn't allocate more memory to create a Movie\n"); 
		return NULL; 
	}
	// TODO: Populate? 
	return mov; 
}

void DestroyMovie(Movie* movie) {
  free(movie); 
}

Movie* CreateMovieFromRow(char *dataRow) {
  Movie* mov = CreateMovie();
  if (mov == NULL) {
    printf("Couldn't create a Movie.\n");
    return NULL;
  }
  int numFields = 9; 
  
  char *token[numFields];
  char *rest = dataRow;
  
  for (int i=0; i<numFields; i++) {
    token[i] = strtok_r(rest, "|", &rest);
    if (token[i] == NULL) {
      fprintf(stderr, "Error reading row\n"); 
    }
  }
  
  mov->id = token[0];
  mov->type = token[1];
  mov->title = token[2]; 
  mov->isAdult = atoi(token[4]); 
  mov->year = atoi(token[5]);
  mov->runtime = atoi(token[7]); 
  mov->genres = token[8]; 
  
  return mov; 
}

