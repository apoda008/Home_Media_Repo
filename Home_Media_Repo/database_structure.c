#include "Database_structure.h"

DatabaseStructure* Construct_Database_Structure(size_t movie_count, size_t series_count) {
	DatabaseStructure* db_structure = (DatabaseStructure*)malloc(sizeof(DatabaseStructure));
	if (!db_structure) {
		fprintf(stderr, "Memory allocation failed for DatabaseStructure\n");
		return NULL;
	}
	db_structure->movies = (MovieTable*)malloc(sizeof(MovieTable));
	db_structure->series = (SeriesTable*)malloc(sizeof(SeriesTable));
	if (!db_structure->movies || !db_structure->series) {
		fprintf(stderr, "Memory allocation failed for MovieTable or SeriesTable\n");
		free(db_structure);
		return NULL;
	}

	db_structure->movie_set_size = movie_count * 2; // Initial size for movies
	db_structure->series_set_size = series_count * 2; // Initial size for series

	//Movie Table
	db_structure->movies->id = (int*)malloc(movie_count * sizeof(int));
	db_structure->movies->title = (char*)malloc(movie_count * 256 * sizeof(char)); // Assuming max title length of 256
	db_structure->movies->description = (char*)malloc(movie_count * 2000 * sizeof(char)); // Assuming max description length of 2000
	db_structure->movies->dir_position = (TCHAR*)malloc(movie_count * MAX_PATH * sizeof(TCHAR)); // Assuming max path length of MAX_PATH
	if (!db_structure->movies->id || !db_structure->movies->title || !db_structure->movies->description || !db_structure->movies->dir_position) {
		fprintf(stderr, "Memory allocation failed for one or more fields in MovieTable\n");
		free(db_structure->movies);
		free(db_structure);
		return NULL;
	}

	//Series Table
	db_structure->series->id = (int*)malloc(series_count * sizeof(int));
	db_structure->series->title = (char*)malloc(series_count * 256 * sizeof(char)); // Assuming max title length of 256
	db_structure->series->description = (char*)malloc(series_count * 2000 * sizeof(char)); // Assuming max description length of 2000
	db_structure->series->season = (int*)malloc(series_count * sizeof(int));
	db_structure->series->episode = (int*)malloc(series_count * sizeof(int));
	if (!db_structure->series->id || !db_structure->series->title || !db_structure->series->description ||
		!db_structure->series->season || !db_structure->series->episode) {
		fprintf(stderr, "Memory allocation failed for one or more fields in SeriesTable\n");
		free(db_structure->series);
		free(db_structure->movies);
		free(db_structure);
		return NULL;
	}
	
	return db_structure;
}

void Insert_Movie(DatabaseStructure* db_structure, int id, const char* title, const char* description, TCHAR* dir_pos) {
	if ((db_structure->movies->num_elements_MV - db_structure->series_set_size) <= 100) {
		printf("Resizing movie table...\n");
		//TODO: Implement resizing logic for the movie table
	}
	
	if ((db_structure == NULL) || (title == NULL) || (description == NULL)) {
		fprintf(stderr, "Database structure, title, or description is NULL\n");
		return;
	}
	db_structure->movies->id[db_structure->movies->num_elements_MV] = id;
	//DELETE: want to do experiment here
	strcpy_s(db_structure->movies->title[id], 256, title);
	printf("Title: %s\n", db_structure->movies->title[id]);	

	strcpy_s(db_structure->movies->title + (db_structure->movies->num_elements_MV * 256), 256, title);
	printf("Title: %s\n", db_structure->movies->title + (db_structure->movies->num_elements_MV * 256));

	strcpy_s(db_structure->movies->description + (db_structure->movies->num_elements_MV * 2000), 2000, description);
	_tcscpy_s(db_structure->movies->dir_position + (db_structure->movies->num_elements_MV * MAX_PATH), MAX_PATH, dir_pos);

}

