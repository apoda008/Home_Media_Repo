#include "api_functions.h"


//WE ARE BUILDING A TRIE BABY!!!
//If more conditionals are needed, add them to the TrieNode root array
//condsidering hash table for the first char of the string command
static TrieNode Trie_Root[8] = { 
	{ NULL, NULL, NULL, 'S', -1}, 
	{ NULL, NULL, NULL, 'C', -1 }, 
	{ NULL, NULL, NULL, 'R', -1 }, 
	{ NULL, NULL, NULL, 'S', -1 }, 
	{ NULL, NULL, NULL, 'T', -1 }, 
	{ NULL, NULL, NULL, 'D', -1 }, 
	{ NULL, NULL, NULL, 'G', -1 },
	{ NULL, NULL, NULL, '*', 8 },
}; // Initialize the root of the Trie for each command

void Build_DB_Trie(){
	Insert_String_Trie(Trie_Root, "SELECT", SELECT);
	Insert_String_Trie(Trie_Root, "CHANGE", CHANGE);
	Insert_String_Trie(Trie_Root, "REMOVE", REMOVE);
	Insert_String_Trie(Trie_Root, "SEARCH", SEARCH);
	Insert_String_Trie(Trie_Root, "TITLE", TITLE);
	Insert_String_Trie(Trie_Root, "DESCRIPTION", DESCRIPTION);
	Insert_String_Trie(Trie_Root, "GENRE", GENRE);
	
	//The last switch case if already made in intialization
	//Insert_String_Trie(Trie_Root, "*", ALL);	

	////TEST DELETE
	//TrieNode* test = &Trie_Root[0];
	//for (int i = 0; i < 6; i++) {
	//	if (test == NULL) {
	//		printf("test @ i:%d is NULL\n", i);
	//		return;
	//	}

	//	printf("test Char: %c\n", test->letter);
	//	printf("test Switch Case: %d\n", test->switch_case);
	//	
	//	test = test->next_l; // Move to the next left node
	//}
}

//OLD FUNCTION
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

//OLD FUNCTION
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

//DONT THINK I WILL USE THIS 
//WILL DELETE
int Validate(const char* token) {
	/*
	* This function will validate the token and return an int value
	* that will be used in the switch statement later on
	*/ 
	int size = strlen(token);

	for(int i = 0 ; i < 8; i++) {
		if(Trie_Root[i].letter == token[0]) {
			//DELETE
			//printf("Token: %s\n", token);
			//printf("Trie Root: %c\n", Trie_Root[i].letter);
			TrieNode* current = &Trie_Root[i];
			for (int j = 1; j < size; j++) {
				if( (token[j] == current->letter) && (current->switch_case > 0) && (i == size - 1) ) {
					return current->switch_case;
				}
				else if (token[j] == current->letter) {
					current = current->next_l; // Move to the next left node
				}
				else {
					return -1; // Invalid token
				}
			}
		}
	} 
}

int Recursive_Validate(const char* token, TrieNode* current, int array_pos) {
	/*
	* This function will validate the token and return an int value
	* that will be used in the switch statement later on
	*/ 

	//BASE CASES
	if (current == NULL) {
			return -1; // If the current node is NULL, return -1
		}
	if (current->switch_case >= 0) {
		return current->switch_case; // Return the switch case if we reach the end of the token
	}
	if( array_pos >= strlen(token) - 1) {
		return -8; // If we reach the end of the token without finding a match, return -1
	}
	if( current->letter != toupper(token[array_pos]) ) {
		return -2; // If the current letter does not match the token letter, return -1
	}

	//Prepare for next recursion
	array_pos += 1; // Move to the next character in the token

	char char_compare = toupper(token[array_pos]); // Convert to uppercase for case-insensitive comparison

	//This can be streamlined
	if(current->next_l == NULL && current->next_m == NULL && current->next_r == NULL) {
		printf("NULL NODES\n");
		return -9; // If any of the next nodes are NULL, return -1
	}
	
	
	if(current->next_l != NULL) {
		if(current->next_l->letter == char_compare) {
			return Recursive_Validate(token, current->next_l, array_pos);
		}
	}
	if (current->next_m != NULL) {
		if (current->next_m->letter == char_compare) {
			return Recursive_Validate(token, current->next_m, array_pos);
		}
	}
	if (current->next_r != NULL) {
		if (current->next_r->letter == char_compare) {
			return Recursive_Validate(token, current->next_r, array_pos);
		}
	}

	//Safety catch
	printf("String placement\n");
	return -10; // If we reach here, the token is invalid
}

int* Query_Transform(char* query_string) {
	/*TODO
	* Tansforms the query string into an int array for use later in switches
	* this will be a long segment of code since it will have to do a lot
	* needs to account for the string input from user ex: "SELECT TITLE FROM MOVIES WHERE TITLE = '->Inception<-'"
	* 
	*/
	//printf("Query Transform called with: %s\n", query_string);
	//int total[8] = { -15, -15, -15, -15, -15, -15, -15, -15 };

	if(query_string == NULL ) {
		fprintf(stderr, "Query string is NULL\n");
		return NULL;
	}

	int* int_array = malloc(8 * sizeof(int));
	if (int_array == NULL) {
		fprintf(stderr, "Memory allocation failed for int_array\n");
		return NULL;
	}

	//DELETE
	char query_string2[256] = "SELECT%TITLE%WHERE%TITLE%some string";

	char *context = NULL;
	char *token = strtok_s(query_string2, "%", &context);
	printf("First token: %s\n", token);
	
	
	for (int i = 0; token != NULL; i++) {
		
		char comparable = toupper(token[0]); // Convert to uppercase for case-insensitive comparison
		
		for (int j = 0; j < 8; j++) {
			if (Trie_Root[j].letter == comparable) {
				printf("i = %d\n", i);
				TrieNode* current = &Trie_Root[j];
				int_array[i] = Recursive_Validate(token, current, 0);
				printf("total[%d] = %d\n", i, int_array[i]);
				break;
			}
		}

		//this requires initialization of the array to -15
		if (int_array[i] == -15) {
			//at this point we could terminate the entire query since an invalid command was found
			//but for now we will just set it to -1. And remember that this will catch "some string"
			//or any input that is not a command. So catches will have to implemented to catch this 
			//if this decides to terminate here. UPDATE
			int_array[i] = -1; // If no match found, set to -1 (invalid)
		}
		
		token = strtok_s(NULL, "%", &context);
	}

	//TESTING ARRAY 

	for (int l = 0; l < 8; l++) {
		printf("Command %d: %d\n", l, int_array[l]);
	}


	return int_array;
}

int* Query_Transform2(const char* s) {
	int* a = malloc(8 * sizeof(int));
	for (int i = 0;i < 8;i++) a[i] = 1000 + i;
	return a;
}

//OLD FUNCTION
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