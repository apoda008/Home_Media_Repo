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
	db_structure->movies->title = (char*)malloc(db_structure->movie_set_size * sizeof(char[256])); // Assuming max title length of 256
	db_structure->movies->description = (char*)malloc(db_structure->movie_set_size * sizeof(char[2000])); // Assuming max description length of 2000
	db_structure->movies->dir_position = (TCHAR*)malloc(db_structure->movie_set_size * sizeof(TCHAR[256])); // Assuming max path length of MAX_PATH
	db_structure->movies->video_size = (int*)malloc(db_structure->movie_set_size * sizeof(__int64)); // Assuming video size is an integer
	db_structure->movies->num_elements_MV = 0;

	if (!db_structure->movies->id || !db_structure->movies->title || !db_structure->movies->description || !db_structure->movies->dir_position) {
		fprintf(stderr, "Memory allocation failed for one or more fields in MovieTable\n");
		free(db_structure->movies);
		free(db_structure);
		return NULL;
	}

	//set values to prevent garbage values
	for (int i = 0; i < db_structure->movie_set_size; i++) {
		db_structure->movies->id[i] = -1; // Initialize IDs to -1 to indicate empty slots
		db_structure->movies->title[i][0] = '\0'; // Initialize titles to empty strings
		db_structure->movies->description[i][0] = '\0'; // Initialize descriptions to empty strings
		_tcscpy_s(db_structure->movies->dir_position[i], 256, _T("")); // Initialize directory positions to empty strings
		db_structure->movies->video_size[i] = 0; // Initialize video sizes to 0
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

MovieTable* Resize_Movie_Table(DatabaseStructure* db, MovieTable* movies) {
	if (movies == NULL) {
		fprintf(stderr, "MovieTable is NULL\n");
		return NULL;
	}
	int old_size = db->movie_set_size;
	int new_size = 2 * old_size;
	db->movie_set_size = new_size;

	// Realloc each internal array based on the fixed-size array types
	movies->id = (int*)realloc(movies->id, new_size * sizeof(int));
	movies->title = (char (*)[256])realloc(movies->title, new_size * sizeof(char[256]));
	movies->description = (char (*)[2000])realloc(movies->description, new_size * sizeof(char[2000]));
	movies->dir_position = (TCHAR(*)[256])realloc(movies->dir_position, new_size * sizeof(TCHAR[256]));
	movies->video_size = (long*)realloc(movies->video_size, new_size * sizeof(__int64));

	// Check all allocations
	if (!movies->id || !movies->title || !movies->description ||
		!movies->dir_position || !movies->video_size) {
		fprintf(stderr, "Memory reallocation failed for one or more MovieTable fields.\n");
		free(movies->id);
		free(movies->title);
		free(movies->description);
		free(movies->dir_position);
		free(movies->video_size);
		free(movies);
		return NULL;
	}

	// Zero out new regions (safe since we're using fixed-size arrays)
	memset(&movies->id[old_size], 0, (new_size - old_size) * sizeof(int));
	memset(&movies->title[old_size], 0, (new_size - old_size) * sizeof(char[256]));
	memset(&movies->description[old_size], 0, (new_size - old_size) * sizeof(char[2000]));
	memset(&movies->dir_position[old_size], 0, (new_size - old_size) * sizeof(TCHAR[256]));
	memset(&movies->video_size[old_size], 0, (new_size - old_size) * sizeof(__int64));

	return movies;
}

void Insert_Movie(DatabaseStructure* db_structure, const char* title, const char* description, TCHAR* dir_pos, __int64 video_size) {
	//ID can probably be removed
	
	if ((db_structure->movie_set_size - db_structure->movies->num_elements_MV) <= 5) {
		//printf("%d - %d\n", db_structure->movie_set_size, db_structure->movies->num_elements_MV);
		printf("\nResizing movie table...\n\n");
		Resize_Movie_Table(db_structure, db_structure->movies);
	}
	
	if ((db_structure == NULL) || (title == NULL) || (description == NULL)) {
		fprintf(stderr, "Database structure, title, or description is NULL\n");
		return;
	}
	db_structure->movies->id[db_structure->movies->num_elements_MV] = db_structure->movies->num_elements_MV;
	//printf("Movie ID: %d\n", db_structure->movies->id[db_structure->movies->num_elements_MV]);
	
	strcpy_s(db_structure->movies->title[db_structure->movies->num_elements_MV], 256, title);
	//DELETE
	//printf("Title: %s\n", db_structure->movies->title[db_structure->movies->num_elements_MV]);

	strcpy_s(db_structure->movies->description[db_structure->movies->num_elements_MV], 2000, description);
	//DELETE
	//printf("Description: %s\n", db_structure->movies->description[db_structure->movies->num_elements_MV]);

	_tcscpy_s(db_structure->movies->dir_position[db_structure->movies->num_elements_MV], 256, dir_pos);
	//DELETE
	//_tprintf(_T("Dir Position: %s\n"), db_structure->movies->dir_position[db_structure->movies->num_elements_MV]);

	//Assuming video_size is an integer representing the size of the video file
	db_structure->movies->video_size[db_structure->movies->num_elements_MV] = video_size;
	//printf("Video Size (INSIDE INSERT): %I64d\n", db_structure->movies->video_size[db_structure->movies->num_elements_MV]);
	//increments the number of elements in the movie table
	db_structure->movies->num_elements_MV++;

	//LOG
	printf("Inserted movie: %s with ID: %d\n", title, db_structure->movies->id[db_structure->movies->num_elements_MV - 1]);
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

void Free_Movies(MovieTable* movies) {
	if (movies) {
		free(movies->id);
		free(movies->title);
		free(movies->description);
		free(movies->dir_position);
		free(movies->video_size);
		free(movies);
	}
}

void Print_Movie_Table(const MovieTable* movies) {
	if (movies == NULL) {
		fprintf(stderr, "MovieTable is NULL\n");
		return;
	}
	printf("Movie Table:\n");
	printf("| %6s | %-50s | %-5s | %-50s | %20s |\n",
		"ID", "Title", "Desc", "Dir Position", "Size");
	for (int i = 0; i < movies->num_elements_MV; i++) {
		
		if (!movies->title[i]) {
			fprintf(stderr, "Null title at index %d\n", i);
			continue;
		}
		if (!movies->dir_position[i]) {
			fprintf(stderr, "Null dir_position at index %d\n", i);
			continue;
		}
		else {

		printf("| %6d | %-50s | %-5s | %-50s | %20I64d |\n",
			movies->id[i],
			movies->title[i],
			"", // placeholder for description if skipped
			movies->dir_position[i],
			movies->video_size[i]);
		}
	}
	return;
}

void Sort_Movie_Table(DatabaseStructure* db_structure) {
	if (db_structure == NULL || db_structure->movies == NULL) {
		fprintf(stderr, "Database structure or MovieTable is NULL\n");
		return NULL;
	}
	
	MovieTable* new_movies = (MovieTable*)malloc(sizeof(MovieTable));
	if( !new_movies) {
		fprintf(stderr, "Memory allocation failed for new MovieTable\n");
		return NULL;
	}

	new_movies->id = (int*)malloc(db_structure->movie_set_size * sizeof(int));
	new_movies->title = (char*)malloc(db_structure->movie_set_size * sizeof(char[256])); // Assuming max title length of 256
	new_movies->description = (char*)malloc(db_structure->movie_set_size * sizeof(char[2000])); // Assuming max description length of 2000
	new_movies->dir_position = (TCHAR*)malloc(db_structure->movie_set_size * sizeof(TCHAR[256])); // Assuming max path length of MAX_PATH
	new_movies->video_size = (int*)malloc(db_structure->movie_set_size * sizeof(__int64)); // Assuming video size is an integer
	new_movies->num_elements_MV = db_structure->movies->num_elements_MV;

	if (!db_structure->movies->id || !db_structure->movies->title || !db_structure->movies->description || !db_structure->movies->dir_position) {
		fprintf(stderr, "Memory allocation failed for one or more fields in MovieTable\n");
		free(db_structure->movies);
		free(db_structure);
		return;
	}

	int n = db_structure->movie_set_size;
	int dups = 0;
	int id = -1;

	for (int i = 0; i < n; i++) {
		//Set the i item to the new array
		new_movies->id[i] = db_structure->movies->id[i];
		strcpy_s(new_movies->title[i], 256, db_structure->movies->title[i]);
		strcpy_s(new_movies->description[i], 2000, db_structure->movies->description[i]);
		_tcscpy_s(new_movies->dir_position[i], 256, db_structure->movies->dir_position[i]);
		new_movies->video_size[i] = db_structure->movies->video_size[i];
	
		
		for (int j = (i + 1); j < (n - i - 1); j++) {
			if( db_structure->movies->id[j] == -1) {
				//NULL column, skip
				continue; 

			}
			else {
				int res = strcmp(new_movies->title[i], db_structure->movies->title[j]);
				if (res < 0) {
					id = db_structure->movies->id[j];
					strcpy_s(new_movies->title[i], 256, db_structure->movies->title[j]);
					strcpy_s(new_movies->description[i], 2000, db_structure->movies->description[j]);
					_tcscpy_s(new_movies->dir_position[i], 256, db_structure->movies->dir_position[j]);
					new_movies->video_size[i] = db_structure->movies->video_size[j];

				}
				if (res == 0) {
					//if the titles are the same, then it will not change the order
					//tracks duplicates. Will do something with this later
					//UPDATE
					dups++;
					continue;
				}
			}
		}
		//Nulls the one who was discovered to be the lowest value in the old structure 
		//db_structure->movies->id[] = -1; // Mark as processed
		db_structure->movies->id[id] = 0;
		db_structure->movies->title[id][0] = '\0'; // Clear title
		db_structure->movies->description[id][0] = '\0'; // Clear description
		_tcscpy_s(db_structure->movies->dir_position[id], 256, _T("")); // Clear directory position
		db_structure->movies->video_size[id] = 0; // Clear video size

		new_movies->id[i] = i; // Set new ID based on index
	}

	Free_Movies(db_structure->movies); // Free the old movies table
	db_structure->movies = new_movies; // Update the database structure to point to the new sorted movies table

	return;
	
}