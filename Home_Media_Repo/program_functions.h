#ifndef PROGRAM_FUNCTIONS_H
#define PROGRAM_FUNCTIONS_H

#include "master.h"
#include "data_structures.h"


//This is the genre/number relation for Movies from TMDB as of 1/13/2025
enum GenreTypeMovie {
    ACTION = 28,
    ADVENTURE = 12,
    ANIMATION = 16,
    COMEDY = 35,
    CRIME = 80,
    DOCUMENTARY = 99,
    DRAMA = 18,
    FAMILY = 10751,
    FANTASY = 14,
    HISTORY = 36,
    HORROR = 27,
    MUSIC = 10402,
    MYSTERY = 9648,
    ROMANCE = 10749,
    SCIENCE_FICTION = 878,
    TV_MOVIE = 10770,
    THRILLER = 53,
    WAR = 10752,
    WESTERN = 37
};

int Fill_Table_Movies(DatabaseStructure* db_structure, Master_Directory* global_ptr);

#endif // !PROGRAM_FUNCTIONS_H
