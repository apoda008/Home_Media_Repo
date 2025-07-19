#include "api_functions.h"


cJSON* get_media(MediaData** hash_table, const char* title, size_t array_size) {

	//deprecated DELETE
	//MediaNode* new_node = search_linked_list_object(title);

	MediaData* new_node = Search_Hash_Table(hash_table, title, array_size);
	
	if (!new_node) {
		printf("Media not found\n");
		return NULL;
	}
	
	cJSON* json_node = cJSON_CreateObject();
	if (!json_node) {
		printf("JSON initialization failed\n");
		return NULL;
	}
	cJSON_AddNumberToObject(json_node, "db_position", new_node->db_position);
	cJSON_AddStringToObject(json_node, "title", new_node->title);
	cJSON_AddNumberToObject(json_node, "tmdb_id", new_node->tmdb_id);
	cJSON_AddBoolToObject(json_node, "media_type", new_node->media_type);

	cJSON* genre_array = cJSON_CreateIntArray(new_node->genre_types, 19);
	if (!genre_array) {
		printf("Array generation failed\n");
		cJSON_Delete(json_node);
		return NULL;
	}
	cJSON_AddItemToObject(json_node, "genre_types", genre_array);
	cJSON_AddStringToObject(json_node, "description", new_node->description);
	cJSON_AddStringToObject(json_node, "dir_position_media", new_node->dir_position_media);

	//DELETE
	//char* j_print = cJSON_Print(json_node);
	//printf("JSON: %s", j_print);

	return json_node;

}




cJSON* input_string_parsing(MediaData** hash_table, char* user_input, size_t array_size) {
	/*
	TODO:
	this will be adjusted after completion of skeleton app server and app
	ADD
	RENAME
	CHANGE DESCRIPTION
	DELETE
	GET

	*/
	char* context;
	char* token = strtok_s(user_input, " ", &context);

	//DELETE
	printf("1st token: %s\n", token);

	int tracker = 0;
	while (token != NULL) {
		if (strcmp(token, "ADD") == 0) {
			//TODO  
			break;
		}
		if (strcmp(token, "CHANGE") == 0) {
			token = strtok_s(NULL, " ", &context);
			if (strcmp(token, "DESCRIPTION") == 0) {
				//TODO
				break;
			}
			if (strcmp(token, "TITLE") == 0) {
				break;
				//TODO
			}
			if (strcmp(token, "GENRE") == 0) {
				break;
				//TODO
			}
		}
		if (strcmp(token, "DELETE") == 0) {
			//TODO
		}
		if (strcmp(token, "GET") == 0) {
			/*Next token should be "Title, discription, genre
			* type, etc.
			*/
			token = strtok_s(NULL, " ", &context);

			//DELETE
			printf("2nd Token %s\n", token);

			if (strcmp(token, "TITLE") == 0) {
				//This will be the title
				token = strtok_s(NULL, " ", &context);

				//DELETE
				printf("3rd token: %s\n", token);

				cJSON* parsed_to_json = get_media(hash_table, token, array_size);



				return parsed_to_json;


			}
		}
		tracker += 1;
		token = strtok_s(NULL, " ", &context);
	}
}