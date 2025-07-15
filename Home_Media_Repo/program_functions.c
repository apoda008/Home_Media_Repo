#include "program_functions.h"
#include "networking.h"

//returns head of linked list of MediaData
//More than likely needs an entire restructuring 
//into a hash table or binary tree
MediaNode* Bin_Read(char* database_file) {


	printf("DATABASE FILE: %s\n", database_file);
	FILE* file = fopen(database_file, "rb");
	if (file == NULL) {
		perror("Failed to open file");
		return 0;
	}

	MediaNode* head = NULL;
	MediaNode* tail = NULL;
	MediaData temp;

	while ((fread(&temp, sizeof(MediaData), 1, file)) == 1) {
		MediaNode* new_node = (MediaNode*)malloc(sizeof(MediaNode));
		if (new_node == NULL) {
			perror("MediaData Memory allocation failed");
			fclose(file);
			return 0;
		}

		memcpy(&new_node->data, &temp, sizeof(MediaData));
		new_node->next = NULL;

		if (head == NULL) {
			head = new_node;
			tail = new_node;
		}
		else {
			tail->next = new_node;
			tail = new_node;
		}
		//DELETE
		printf("Title order: %s\n", new_node->data.title);

		for (int i = 0; i < 19; i++) {

		}
	}

	fclose(file);
	return head;
}

void media_write(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, TCHAR dir_position, Master_Directory* global_ptr) {
	char* first_char_string = title->valuestring;
	TCHAR filename[8] = _T("\\a.bin");

	//because strcat is a bitch
	filename[1] = tolower(first_char_string[0]);

	TCHAR file_buffer[MAX_PATH];
	_tcscpy_s(file_buffer, MAX_PATH, global_ptr->movie_bin_path);
	_tcscat_s(file_buffer, MAX_PATH, filename);
	
	//(OLD)strcat_s(file_buffer, MAX_PATH, create_folder_location);
	//(OLD)strcat_s(file_buffer, MAX_PATH, "\\");

	//(OLD)set global bin file path for quick reference 
	//(OLD)strcpy_s(master_pathing.movie_bin_path, MAX_PATH, file_buffer);

	//(OLD)strcat_s(file_buffer, MAX_PATH, filename);

	//DELETE BE AWARE; TEMP IS NOT FREED. 
	_tprintf(_T("FILE_BUFFER: %s\n"), file_buffer);

	//(OLD)set global bin file path for quick reference 

	//(OLD)printf("GLOBAL DIR PATH: %s", master_pathing.movie_bin_path);
	//(OLD)_tprintf(_T("GLOBAL DIR PATH: %s\n"), master_pathing.movie_bin_path);

	bool is_movie = false;

	if (!strcmp(media_type->valuestring, "movie")) {
		is_movie = true;
	}

	MediaData temp = { 52, "", id->valuedouble, is_movie, {0}, "",_T("")};

	strcpy_s(temp.title, 256, title->valuestring);
	strcpy_s(temp.description, 2000, description->valuestring);
	//strcpy_s(temp.dir_position_media, MAX_PATH, dir_position);
	_tcscpy_s(temp.dir_position_media, MAX_PATH, &dir_position);
	
	cJSON* genre_number;
	int i = 0;
	cJSON_ArrayForEach(genre_number, genre_ids) {
		temp.genre_types[i] = (int)genre_number->valuedouble;
		i++;
	}

	//DELETE
	printf("\n");

	cJSON_Delete(genre_number);
	////////////////FILE WRITE///////////////////////
	//MAKE THIS ITS OWN FUNCTION
	FILE* file = _tfopen(file_buffer, _T("ab"));

	if (file == NULL) {
		perror("error opening file");
		return 0;
	}

	size_t written = fwrite(&temp, sizeof(MediaData), 1, file);

	if (written != 1) {
		perror("fwrite failed \n");
		printf("Written: %zu\n", written);

		if (ferror(file)) {
			fprintf(stderr, "I/O error while writing \n");
		}
	}

	fclose(file);

	//genre bin write
	//for (int i = 0; i < 19; i++) {
	//	//DELETE
	//	//printf("GENRE: %d\n", temp.genre_types[i]);

	//	switch (temp.genre_types[i]) {
	//	case ACTION:
	//		genre_write("ACTION", temp.title);
	//		break;
	//	case ADVENTURE:
	//		genre_write("ADVENTURE", temp.title);
	//		break;
	//	case ANIMATION:
	//		genre_write("ANIMATION", temp.title);
	//		break;
	//	case COMEDY:
	//		genre_write("COMEDY", temp.title);
	//		break;
	//	case CRIME:
	//		genre_write("CRIME", temp.title);
	//		break;
	//	case DOCUMENTARY:
	//		genre_write("DOCUMENTARY", temp.title);
	//		break;
	//	case DRAMA:
	//		genre_write("DRAMA", temp.title);
	//		break;
	//	case FAMILY:
	//		genre_write("FAMILY", temp.title);
	//		break;
	//	case FANTASY:
	//		genre_write("FANTASY", temp.title);
	//		break;
	//	case HISTORY:
	//		genre_write("HISTORY", temp.title);
	//		break;
	//	case HORROR:
	//		genre_write("HORROR", temp.title);
	//		break;
	//	case MUSIC:
	//		genre_write("MUSIC", temp.title);
	//		break;
	//	case MYSTERY:
	//		genre_write("MYSTERY", temp.title);
	//		break;
	//	case ROMANCE:
	//		break;
	//		genre_write("ROMANCE", temp.title);
	//	case SCIENCE_FICTION:
	//		genre_write("SCIENCE_FICTION", temp.title);
	//		break;
	//	case TV_MOVIE:
	//		genre_write("TV_MOVIE", temp.title);
	//		break;
	//	case THRILLER:
	//		genre_write("THRILLER", temp.title);
	//		break;
	//	case WAR:
	//		genre_write("WAR", temp.title);
	//		break;
	//	case WESTERN:
	//		genre_write("WESTER", temp.title);
	//		break;
	//	}
	//}
}//end of media_write
