#include "program_functions.h"
//temp
#include "data_structures.h"

long GetVideoSize(FILE* video_file) {
	fseek(video_file, 0, SEEK_END);
	return ftell(video_file);
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

void Fill_Table_Movies(DatabaseStructure* db_structure, Master_Directory* global_ptr) {
	if (db_structure == NULL || global_ptr == NULL) {
		fprintf(stderr, "Database structure or global pointer is NULL\n");
		return;
	}
	
	FILE* file = _tfopen(global_ptr->movie_bin_path, _T("wb"));
	if (file == NULL) {
		fprintf(stderr, "Failed to open movie bin file for writing\n");
		return;
	}

	MediaData movie;


}