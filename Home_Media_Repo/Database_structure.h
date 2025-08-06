#pragma once
#ifndef DATABASE_STRUCTURE_H
#define DATABASE_STRUCTURE_H
//#include "master.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>



typedef struct {
	//all these sizes to by dynamically allocated at a later date
	int* id;
	char (*title)[256]; 
	char (*description)[2000];
	TCHAR (*dir_position)[256]; //to keep track of the directory position of the movie
	__int64* video_size;
	
	int num_elements_MV; //to keep track of how many elements are in the table

} MovieTable;

typedef struct {
	//all these sizes to by dynamically allocated at a later date
	int* id;
	char (*title);
	char (*description);
	int* season;
	int* episode;

	int num_elements_Se; //to keep track of how many elements are in the table
} SeriesTable;


typedef struct {
	MovieTable* movies;
	SeriesTable* series;

	int movie_set_size;
	int series_set_size;
} DatabaseStructure;

DatabaseStructure* Construct_Database_Structure(size_t movie_count, size_t series_count);

void Insert_Movie(DatabaseStructure* db_structure, const char* title, const char* description, TCHAR* dir_pos, __int64 video_size);

//Worry about it last 
void Free_Database_Structure(DatabaseStructure* db_structure);

void Print_Movie_Table(const MovieTable* movies);
#endif // !DATABASE_STRUCTURE_H

