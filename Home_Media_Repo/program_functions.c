#include "program_functions.h"
//temp
#include "data_structures.h"

long GetVideoSize(FILE* video_file) {
	fseek(video_file, 0, SEEK_END);
	long result = ftell(video_file);
	rewind(video_file); 
	return result;
}

void genre_write(const char* genre, const char* title, int index) {
	
}


//void Genre_Mapping(MediaData** hash_table, Master_Directory* global_ptr) {
//	//genre bin write
//	for (int i = 0; i < global_ptr->size_of_hash ; i++) {
//		//DELETE
//		//printf("GENRE: %d\n", temp.genre_types[i]);
//		if (hash_table[i] != NULL) {
//			
//			for (int j = 0; j < 19; j++) {
//				switch (hash_table[i]->genre_types[j]) {
//				case ACTION:
//					genre_write("ACTION", hash_table[j]->title, i);
//					break;
//				case ADVENTURE:
//					genre_write("ADVENTURE", hash_table[j]->title, i);
//					break;
//				case ANIMATION:
//					genre_write("ANIMATION", hash_table[j]->title, i);
//					break;
//				case COMEDY:
//					genre_write("COMEDY", hash_table[j]->title, i);
//					break;
//				case CRIME:
//					genre_write("CRIME", hash_table[j]->title, i);
//					break;
//				case DOCUMENTARY:
//					genre_write("DOCUMENTARY", hash_table[j]->title, i);
//					break;
//				case DRAMA:
//					genre_write("DRAMA", hash_table[j]->title, i);
//					break;
//				case FAMILY:
//					genre_write("FAMILY", hash_table[j]->title, i);
//					break;
//				case FANTASY:
//					genre_write("FANTASY", hash_table[j]->title, i);
//					break;
//				case HISTORY:
//					genre_write("HISTORY", hash_table[j]->title, i);
//					break;
//				case HORROR:
//					genre_write("HORROR", hash_table[j]->title, i);
//					break;
//				case MUSIC:
//					genre_write("MUSIC", hash_table[j]->title, i);
//					break;
//				case MYSTERY:
//					genre_write("MYSTERY", hash_table[j]->title, i);
//					break;
//				case ROMANCE:
//					break;
//					genre_write("ROMANCE", hash_table[j]->title, i);
//				case SCIENCE_FICTION:
//					genre_write("SCIENCE_FICTION", hash_table[j]->title, i);
//					break;
//				case TV_MOVIE:
//					genre_write("TV_MOVIE", hash_table[j]->title, i);
//					break;
//				case THRILLER:
//					genre_write("THRILLER", hash_table[j]->title, i);
//					break;
//				case WAR:
//					genre_write("WAR", hash_table[j]->title, i);
//					break;
//				case WESTERN:
//					genre_write("WESTERN", hash_table[j]->title, i);
//					break;
//				default:
//					break;
//				}
//			}
//		}
//	}
//}

int Fill_Table_Movies(DatabaseStructure* db_structure, Master_Directory* global_ptr) {
	if (db_structure == NULL || global_ptr == NULL) {
		fprintf(stderr, "Database structure or global pointer is NULL\n");
		return;
	}
	
	char* alpha = "abcdefghijklmnopqrstuvwxyz";
	
	//iterates throught the bin folder files Ex: a.bin, b.bin, c.bin, etc.
	for (int i = 0; i < strlen(alpha); i++) {
		TCHAR* path[MAX_PATH];
		_stprintf_s(path, MAX_PATH, _T("%s\\%c.bin"), global_ptr->movie_bin_path, alpha[i]);
		_tprintf(_T("Reading movie bin file: %s\n"), path);
		FILE* file = _tfopen(path, _T("rb"));
		if (file == NULL) {
			fprintf(stderr, "Failed to open movie bin file for writing\n");
			return 0;
		}

		MediaData movie;

		//iterates through the bin file and reads each movie
		while ((fread(&movie, sizeof(MediaData), 1, file)) == 1) {
			
			//Might use this if style statement to make it usable for 
			// both movies and series
			//if (movie.media_type == true) { }
			printf("Movie title: %d\n", movie.title);

			long movie_size = GetVideoSize(movie.dir_position_media);
			Insert_Movie(db_structure, movie.title, movie.description, movie.dir_position_media, movie_size);
			printf("Movie Title IN TABLE:: %s\n", db_structure->movies->title[db_structure->movies->num_elements_MV]);
			
		}

	}
	return 1;
}