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

void Better_Print_Table(const MovieTable* movies) {
	if (movies == NULL) {
		fprintf(stderr, "MovieTable is NULL\n");
		return;
	}
	if( movies->num_elements_MV == 0) {
		printf("Movie table is empty.\n");
		return;
	}

	printf("Movie Table:\n");
	printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("| %6s | %-50s | %-50s | %-50s | %20s |\n",
		"ID", "Title", "Description", "Dir Position", "Size");
	printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < movies->num_elements_MV; i++) {
		
		if( movies->id != NULL) {
			printf("| %6d |", movies->id[i]);
		}
		else { printf("| %6s |", "(null)"); }
		
		if( movies->title == NULL) {
			printf(" %-50s |", "(null)");
		}
		else { printf(" %-50s |", movies->title[i]); }
		
		if (movies->description == NULL) {
			printf(" %-50s |", "null");
		}
		else { printf(" %-50.50s |", movies->description[i]);}

		if ( movies->dir_position == NULL || movies->dir_position[i][0] == '\0') {
			printf(" %-50s |", "(null)");
		}
		else { _tprintf(_T(" %-50.50s |"), movies->dir_position[i]); }
		
		if (movies->video_size == NULL) {
			printf(" %20s |\n", "(null)");
		}
		else { printf(" %20I64d |\n", movies->video_size[i]); }
	}
	printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
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
		if( db_structure->movies->id[i] == -1) {
			new_movies->id[i] = n; 
		}
		else if ((new_movies->id[i] == -1) && (db_structure->movies->id[i] != -1) ){
			//so that it does not get stuck in a null column
			new_movies->id[i] = db_structure->movies->id[i];
			strcpy_s(new_movies->title[i], 256, db_structure->movies->title[i]);
			strcpy_s(new_movies->description[i], 2000, db_structure->movies->description[i]);
			_tcscpy_s(new_movies->dir_position[i], 256, db_structure->movies->dir_position[i]);
			new_movies->video_size[i] = db_structure->movies->video_size[i];
		}
		else {
			new_movies->id[i] = db_structure->movies->id[i];
			strcpy_s(new_movies->title[i], 256, db_structure->movies->title[i]);
			strcpy_s(new_movies->description[i], 2000, db_structure->movies->description[i]);
			_tcscpy_s(new_movies->dir_position[i], 256, db_structure->movies->dir_position[i]);
			new_movies->video_size[i] = db_structure->movies->video_size[i];
		
		}
		//THERE AN ISSUE IF IT COPIES A NULL COLUMN IT WILL ALWAYS BE THE LOWEST
		
		//for (int j = (i + 1); j < (n - i - 1); j++) {
		for (int j = 0; j < n; j++) {
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
					//UPDATE: This is tracking far more than it should
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
	//DELETE
	printf("NUMBER OF DUPLICATES: %d\n", dups);

	Free_Movies(db_structure->movies); // Free the old movies table
	db_structure->movies = new_movies; // Update the database structure to point to the new sorted movies table

	return;
	
}

int Save_Database(const DatabaseStructure* database, TCHAR* location) {

	TCHAR file_buffer[_MAX_PATH];
	_tcscpy_s(file_buffer, _MAX_PATH, location);
	_tcscat_s(file_buffer, _MAX_PATH, _T("\\table.bin"));

	FILE* table_file = _tfopen(file_buffer, _T("ab"));
	if (table_file == NULL) {
		perror("Failed to create/open table file\n");
		return -1;
	}

	int total_elements = database->movies->num_elements_MV;
	fwrite(&total_elements, sizeof(int), 1, table_file);

	size_t written = 0;
	for (int i = 0; i < total_elements; i++) {
		written = (fwrite(&database->movies->id[i], sizeof(int), 1, table_file));
		if (written == 0) {
			perror("Failed to write id\n");
			return -1;
		}
		written = (fwrite(&database->movies->title[i], 256, 1, table_file));
		if (written == 0) {
			perror("Failed to write title\n");
			return -1;
		}
		written = (fwrite(&database->movies->description[i], 2000, 1, table_file));
		if (written == 0) {
			perror("Failed to write description\n");
			return -1;
		}
		written = (fwrite(&database->movies->dir_position[i], 256 * sizeof(TCHAR), 1, table_file));
		if (written == 0) {
			perror("Failed to write dir_position\n");
			return -1;
		}
		written = (fwrite(&database->movies->video_size[i], sizeof(__int64), 1, table_file));
		if (written == 0) {
			perror("Failed to write video_size\n");
			return -1;
		}
	}

	fclose(table_file);
	return 0;
}

DatabaseStructure* Read_Into_Table(TCHAR* location) {
	//THIS IS STATIC. WILL NEED A MORE ROBUST WAY TO FIND DYNAMICALLY TODO:
	TCHAR file_buffer[_MAX_PATH];
	_tcscpy_s(file_buffer, _MAX_PATH, location);
	_tcscat_s(file_buffer, _MAX_PATH, _T("\\table.bin"));

	FILE* table_file = _tfopen(file_buffer, _T("rb"));
	if (table_file == NULL) {
		perror("Failed to open for read table file\n");
		return -1;
	}

	size_t read = 0;
	size_t total_elements_to_read = 0;

	fread(&total_elements_to_read, sizeof(int), 1, table_file);

	DatabaseStructure* NewDb = Construct_Database_Structure(total_elements_to_read, 0);
	if (NewDb == NULL) {
		printf("Failed to create table\n");
		return NULL;
	}

	int bytes_read = 0;
	
	for (int j = 0; j < total_elements_to_read; j++) {
		
		bytes_read = fread(&NewDb->movies->id[j], sizeof(int), 1, table_file);
		if (bytes_read == 0) {
			perror("Failed to read id\n");
		}
		
		bytes_read = fread(&NewDb->movies->title[j], 256, 1, table_file);
		if (bytes_read == 0) {
			perror("Failed to read id\n");
		}
		
		bytes_read = fread(&NewDb->movies->description[j], 2000, 1, table_file);
		if (bytes_read == 0) {
			perror("Failed to read id\n");
		}
		
		bytes_read = fread(&NewDb->movies->dir_position[j], 256 * sizeof(TCHAR), 1, table_file);
		if (bytes_read == 0) {
			perror("Failed to read id\n");
		}
		
		bytes_read = fread(&NewDb->movies->video_size[j], sizeof(__int64), 1, table_file);
		if (bytes_read == 0) {
			perror("Failed to read id\n");
		}

		
	}
	NewDb->movies->num_elements_MV = total_elements_to_read;
	fclose(table_file);
	return NewDb;

}

void Movie_Table_Copy(const MovieTable* source, MovieTable* dest) {
	if (source == NULL || dest == NULL) {
		fprintf(stderr, "Source or destination MovieTable is NULL\n");
		return;
	}
	dest->num_elements_MV = source->num_elements_MV;
	// Allocate memory for destination arrays
	dest->id = (int*)malloc(source->num_elements_MV * sizeof(int));
	dest->title = (char(*)[256])malloc(source->num_elements_MV * sizeof(char[256]));
	dest->description = (char(*)[2000])malloc(source->num_elements_MV * sizeof(char[2000]));
	dest->dir_position = (TCHAR(*)[256])malloc(source->num_elements_MV * sizeof(TCHAR[256]));
	dest->video_size = (long*)malloc(source->num_elements_MV * sizeof(__int64));
	if (!dest->id || !dest->title || !dest->description || !dest->dir_position || !dest->video_size) {
		fprintf(stderr, "Memory allocation failed for one or more fields in destination MovieTable\n");
		free(dest->id);
		free(dest->title);
		free(dest->description);
		free(dest->dir_position);
		free(dest->video_size);
		return;
	}
	// Copy data from source to destination
	for (int i = 0; i < source->num_elements_MV; i++) {
		dest->id[i] = source->id[i];
		strcpy_s(dest->title[i], 256, source->title[i]);
		strcpy_s(dest->description[i], 2000, source->description[i]);
		_tcscpy_s(dest->dir_position[i], 256, source->dir_position[i]);
		dest->video_size[i] = source->video_size[i];
	}
}

//deprecated
void Insert_String_Trie(TrieNode* trie, const char* str, int switch_val) {
	/*
	* String must be ALL CAPITAL LETTERS. Or this function will need adjustments
	* so that any character can be inserted into the trie
	*/
	
	if (trie == NULL || str == NULL) {
		fprintf(stderr, "Trie or string is NULL\n");
		return;
	}
	
	//This matches the first character of the string to the first letter of the trie
	for (int k = 0; k < 8; k++) {
		
		if (trie[k].letter == str[0]) {
			TrieNode* current = &trie[k];
			int size = strlen(str);

			//this builds the trie structure for whatever word is passed in
			for (int i = 1; i < size; i++) {
				TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
				if (newNode == NULL) {
					fprintf(stderr, "Memory allocation failed for TrieNode\n");
					return;
				}

				if (str[i] == '\0') {
					fprintf(stderr, "Invalid character in string at index %d\n", i);
					free(newNode);
					return;
				}

				newNode->next_l = NULL;
				newNode->next_m = NULL;
				newNode->next_r = NULL;
				newNode->letter = str[i];
				
				if (i == size - 1) {
					newNode->switch_case = switch_val; // Set switch case for the last node
					printf("Inserting character '%c' with switch case %d\n", newNode->letter, newNode->switch_case);
				}
				else {
					newNode->switch_case = -1; // Set switch case for intermediate nodes
				}

				if (current->next_l == NULL) {
					current->next_l = newNode; // Insert as left child if no left child exists
				}
				else if (current->next_m == NULL) {
					current->next_m = newNode; // Insert as middle child if no middle child exists
				}
				else if (current->next_r == NULL) {
					current->next_r = newNode; // Insert as right child if no right child exists
				}
				else {
					fprintf(stderr, "Trie is full, cannot insert more nodes\n");
					free(newNode);
					return;
				}

				current = newNode; // Move to the newly created node
			}
		}
	}
}
//deprecated
void Print_Trie(TrieNode* trie) {
	if (trie == NULL) {
		fprintf(stderr, "Trie is NULL\n");
		return;
	}
	printf("Trie Structure:\n");
	for (int i = 0; i < 8; i++) {
		if (trie[i].letter != '\0') {
			printf("Root %d: %c\n", i, trie[i].letter);
			// You can add more logic to traverse and print the entire trie structure
		}
	}
}




