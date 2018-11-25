#ifndef MOVIE_H
#define MOVIE_H


//=============================
//
// Helper struct.
// Holds the fields of a movie
// provided in the data files. 
//
//=============================

typedef struct movie {
	char *id; 
	char *type; 
	char *title; 
	int isAdult; 
	int year; 
	int runtime; 
	char *genres;
} Movie; 

// Mallocs a movie. 
Movie* CreateMovie(); 

// Clears and frees a malloc'd movie. 
void DestroyMovie(Movie* movie);

// Creates a movie struct and populates
// it with the data in a provided row. 
Movie* CreateMovieFromRow(char *dataRow); 

#endif // MOVIE
