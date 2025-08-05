#include "api_functions.h"

//long GetVideoSize(FILE* video_file) {
//	fseek(video_file, 0, SEEK_END);
//	return ftell(video_file);
//}
TCHAR* Video_Transcode_Mp4(TCHAR* video_path) {
	//This will be used to transcode the video into mp4 format
	//which is supported by the C# app
	//this will be a placeholder for now

	//TODO:
	//need to do this for every format that is in the Media Library 
	//(i.e. avi, mkv, etc.)

	//this needs to change the dir paths for both input and output. Then the
	//Transcoded video needs to be moved to the Temp folder. and the TCHAR to 
	//that new location passed out of this function. Meaning that the new TCHAR 
	//will need to be MALLOC'ed and then free'd after the movie has been transfered
	//to the C# app. Look into setting a timer for the transcoded video to be deleted
	//or reformatting the entire video library to mp4 format which can probably be done 
	//at the start of the program. 
	const char* cmd = "ffmpeg -i \"C:\\Users\\Dan\\Videos\\input.mov\" -c:v libx264 -c:a aac -movflags +faststart \"C:\\Users\\Dan\\Videos\\output.mp4\"";
	
	int result = system(cmd);

	if (result != 0) 
	{
		printf("Video transcoding failed\n");
		return NULL;
	}
	else {
		printf("Video transcoding successful\n");
		TCHAR* output_video = _T("output_video.mp4");
		return output_video; //return the path to the transcoded video
	}
	
	return NULL;
}

cJSON* Get_All_Media(MediaData** hash_table, const char* title, size_t array_size) {

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
	//cJSON_AddStringToObject(json_node, "dir_position_media", new_node->dir_position_media);

	//DELETE
	//char* j_print = cJSON_Print(json_node);
	//printf("JSON: %s", j_print);

	return json_node;

}

void Input_Parse(SOCKET client_socket, MediaData** hash_table, char* user_input, size_t array_size) {

	//It would appear that the data structure component of this needs to be completely reworked in 
	//order to make this more in line with a database structure. 

	char* context; 
	char* token = strtok_s(user_input, "%", &context);

	//PRIMARY COMMANDS 
	if (strcmp(token, "GET") == 0) {
		//DELETE
		printf("Primary Command: %s\n", token);
		token = strtok_s(NULL, "%", &context);

		if (strcmp(token, "MOVIE") == 0) {
			//DELETE
			printf("Major Category: %s\n", token);
			token = strtok_s(NULL, "%", &context);

			if (strcmp(token, "WHERE") == 0) {
				//DELETE
				printf("Identifier: %s\n", token);
				token = strtok_s(NULL, "%", &context);

				if (strcmp(token, "TITLE") == 0) {
					//DELETE
					printf("Minor Category: %s\n", token);
					token = strtok_s(NULL, "%", &context);

					if (token != NULL) {
						cJSON* result = Get_All_Media(hash_table, token, array_size);
						char* j_print = cJSON_Print(result);
						send(client_socket, j_print, strlen(j_print), 0);
						cJSON_Delete(result); //Free the JSON object after sending
					}
				}
			}
		}
	}
}

cJSON* Input_String_Parsing(MediaData** hash_table, char* user_input, size_t array_size) {
	/*
	TODO:
	This will be the focus of work for the coming days. So what needs to happen is to figure out 
	how to format the incoming command string. The primary first commands 

	GET, ADD, CHANGE, DELETE 

	string input format will be PRIMARY%MAJOR%IDENTIFIER%MINOR%EQUALITY%VALUE%. This helps with 
	tokenization and prevents mix up with the spaces that are naturally in string information

	In essence, SQL statements are built from a small set of keywords and clauses that you string 
	together in a defined order. You tell the database:

	What you want (SELECT, INSERT, etc.)

	Where to get it (FROM)

	Which rows to include (WHERE, HAVING)

	How to group or sort the results (GROUP BY, ORDER BY)

	and—with this consistent pattern—you can express virtually any relational operation.



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
			/*Next token should be "Title, description, genre
			* type, etc.
			*/
			token = strtok_s(NULL, " ", &context);

			//DELETE
			printf("2nd Token %s\n", token);

			if (strcmp(token, "TITLE") == 0) {
				//This will be the title
				//token = strtok_s(NULL, " ", &context);

				//DELETE
				//Problem: This will sometimes return with a break when 
				//coming from C# server
				printf("Remaining token: %s\n", context);

				cJSON* parsed_to_json = Get_All_Media(hash_table, context, array_size);



				return parsed_to_json;
			}
			if (strcmp(token, "VIDEO") == 0) {
				//this will open and start a video stream to the requeser (C# app)
			}
		}
		tracker += 1;
		token = strtok_s(NULL, " ", &context);
	}
}

int Stream_Video(SOCKET client_socket, MediaData** hash_table, size_t array_size, char* title) {
	
	//Needs to check if its a valid command
	printf("Streaming video for title: %s\n", title);
	
	MediaData* target_vid = hash_table[Hash_Function(title, array_size)];
	if ((target_vid == NULL)) {
		printf("Video not found in hash table.\n");
		return 1; //Error code for video not found
	}
	
	//TODO:
	//at some point there should be a conversion from whatever video format the video is in into 
	///mp4 which is natively supported by the C# app (HTML5)
	//And just a heads up so that you can hate yourself more. At some point, probably soon you're gonna 
	// have to self implement REST so have fun with that 
	//TCHAR* transcoded_video = Video_Transcode_Mp4(target_vid->dir_position_media);
	
	FILE* video_file = _tfopen(target_vid->dir_position_media, _T("rb"));
	
	//May not need if I decide to restructure this whole DB
	//long video_size = GetVideoSize(video_file);

	void* large_buffer = malloc(5000);
	if (large_buffer == NULL) {
		printf("Memory allocation failed for large buffer.\n");
		fclose(video_file);
		return 1; //Error code for memory allocation failure
	}
	size_t bytes_read;
	int bytes_packet = 1;
	while ((bytes_read = fread(large_buffer, 1, 5000, video_file)) > 0) {
		printf("Sending packet[%d] with %zu bytes\n", bytes_packet, bytes_read);
		int bytes_sent = send(client_socket, large_buffer, bytes_read, 0);
		if (bytes_sent == SOCKET_ERROR) {
			printf("Failed to send video data: %d\n", WSAGetLastError());
			free(large_buffer);
			fclose(video_file);
			return 1; //Error code for send failure
		}
		bytes_packet++;
		recv(client_socket, NULL, 0, 0); //Wait for client to be ready for more data
	}
	fclose(video_file);	

	return 0;
	
}