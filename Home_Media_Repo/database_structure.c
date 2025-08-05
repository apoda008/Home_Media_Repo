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
	db_structure->movies->id = (int*)malloc(db_structure->movie_set_size * sizeof(int));
	//db_structure->movies->title = (char*)malloc(movie_count * 256 * sizeof(char)); // Assuming max title length of 256
	db_structure->movies->title = (char*)malloc(db_structure->movie_set_size * sizeof(char[256])); // Assuming max title length of 256
	//db_structure->movies->description = (char*)malloc(movie_count * 2000 * sizeof(char)); // Assuming max description length of 2000
	db_structure->movies->description = (char*)malloc(db_structure->movie_set_size * sizeof(char[2000])); // Assuming max description length of 2000
	//db_structure->movies->dir_position = (TCHAR*)malloc(movie_count * MAX_PATH * sizeof(TCHAR)); // Assuming max path length of MAX_PATH
	db_structure->movies->dir_position = (TCHAR*)malloc(db_structure->movie_set_size * sizeof(TCHAR[256])); // Assuming max path length of MAX_PATH
	db_structure->movies->video_size = (int*)malloc(db_structure->movie_set_size * sizeof(int)); // Assuming video size is an integer
	db_structure->movies->num_elements_MV = 0;

	if (!db_structure->movies->id || !db_structure->movies->title || !db_structure->movies->description || !db_structure->movies->dir_position) {
		fprintf(stderr, "Memory allocation failed for one or more fields in MovieTable\n");
		free(db_structure->movies);
		free(db_structure);
		return NULL;
	}

	//Series Table
	//Needs correction like the movie table but will do later
	db_structure->series->id = (int*)malloc(series_count * sizeof(int));
	db_structure->series->title = (char*)malloc(series_count * 256 * sizeof(char)); // Assuming max title length of 256
	db_structure->series->description = (char*)malloc(series_count * 2000 * sizeof(char)); // Assuming max description length of 2000
	db_structure->series->season = (int*)malloc(series_count * sizeof(int));
	db_structure->series->episode = (int*)malloc(series_count * sizeof(int));
	db_structure->series->num_elements_Se = 0;
	if (!db_structure->series->id || !db_structure->series->title || !db_structure->series->description ||
		!db_structure->series->season || !db_structure->series->episode) {
		fprintf(stderr, "Memory allocation failed for one or more fields in SeriesTable\n");
		free(db_structure->series);
		free(db_structure->movies);
		free(db_structure);
		return NULL;
	}
	/////////////////////////////////////////////////////////

	return db_structure;
}

MovieTable* Resize_Movie_Table(MovieTable* movies) {
	if (movies == NULL) {
		fprintf(stderr, "MovieTable is NULL\n");
		return NULL;
	}
	int new_size = 2 * movies->num_elements_MV; // Double the size for resizing

	MovieTable* resized_movies = (MovieTable*)realloc(movies, new_size * sizeof(MovieTable));
	if (!resized_movies) {
		fprintf(stderr, "Memory reallocation failed for MovieTable\n");
		return NULL;
	}
	resized_movies->id = (int*)realloc(resized_movies->id, new_size * sizeof(int));
	resized_movies->title = (char*)realloc(resized_movies->title, new_size * sizeof(char[256]));
	resized_movies->description = (char*)realloc(resized_movies->description, new_size * sizeof(char[2000]));
	resized_movies->dir_position = (TCHAR*)realloc(resized_movies->dir_position, new_size * sizeof(TCHAR[256]));
	resized_movies->video_size = (long*)realloc(resized_movies->video_size, new_size * sizeof(long));
	if (!resized_movies->id || !resized_movies->title || !resized_movies->description ||
		!resized_movies->dir_position || !resized_movies->video_size) {
		fprintf(stderr, "Memory reallocation failed for one or more fields in MovieTable\n");
		free(resized_movies);
		return NULL;
	}
	return resized_movies;
}

void Insert_Movie(DatabaseStructure* db_structure, const char* title, const char* description, TCHAR* dir_pos, int video_size) {
	//ID can probably be removed
	
	if ((db_structure->movie_set_size - db_structure->movies->num_elements_MV) <= 5) {
		printf("%d - %d\n", db_structure->movies->num_elements_MV, db_structure->movie_set_size);
		printf("Resizing movie table...\n");
		//TODO: Implement resizing logic for the movie table
	}
	
	if ((db_structure == NULL) || (title == NULL) || (description == NULL)) {
		fprintf(stderr, "Database structure, title, or description is NULL\n");
		return;
	}
	db_structure->movies->id[db_structure->movies->num_elements_MV] = db_structure->movies->num_elements_MV;
	printf("Movie ID: %d\n", db_structure->movies->id[db_structure->movies->num_elements_MV]);
	//DELETE: want to do experiment here. This works
	strcpy_s(db_structure->movies->title[db_structure->movies->num_elements_MV], 256, title);
	//DELETE
	printf("Title: %d\n", db_structure->movies->title[db_structure->movies->num_elements_MV]);

	strcpy_s(db_structure->movies->description[db_structure->movies->num_elements_MV], 2000, description);
	//DELETE
	printf("Description: %s\n", db_structure->movies->description[db_structure->movies->num_elements_MV]);

	_tcscpy_s(db_structure->movies->dir_position[db_structure->movies->num_elements_MV], 256, dir_pos);
	//DELETE
	_tprintf(_T("Dir Position: %s\n"), db_structure->movies->dir_position[db_structure->movies->num_elements_MV]);

	//Assuming video_size is an integer representing the size of the video file
	db_structure->movies->video_size[db_structure->movies->num_elements_MV] = video_size;
	
	//increments the number of elements in the movie table
	db_structure->movies->num_elements_MV++;
}

void Free_Database_Structure(DatabaseStructure* db_structure) {
	if (db_structure) {
		if (db_structure->movies) {
			free(db_structure->movies->id);
			free(db_structure->movies->title);
			free(db_structure->movies->description);
			free(db_structure->movies->dir_position);
			free(db_structure->movies->video_size);
			free(db_structure->movies);
		}
		if (db_structure->series) {
			free(db_structure->series->id);
			free(db_structure->series->title);
			free(db_structure->series->description);
			free(db_structure->series->season);
			free(db_structure->series->episode);
			free(db_structure->series);
		}
		free(db_structure);
	}
}

void Print_Movie_Table(const MovieTable* movies) {
	if (movies == NULL) {
		fprintf(stderr, "MovieTable is NULL\n");
		return;
	}
	printf("Movie Table:\n");
	for (int i = 0; i < movies->num_elements_MV; i++) {
		printf("ID: %d, Title: %s, Description: %s, Dir Position: %s, Video Size: %ld\n",
			movies->id[i], movies->title[i], movies->description[i],
			movies->dir_position[i], movies->video_size[i]);
	}
}