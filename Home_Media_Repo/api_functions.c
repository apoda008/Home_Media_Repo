#include "api_functions.h"

static char title_str[256];

void Select(int enum_designation_obj, int enum_target) {
	if(TITLE == enum_target) {
		//do thing
		printf("Selected title\n");
	}
	else if (DESCRIPTION == enum_target) {
		//do thing
		printf("Selected description\n");
	}
	else if (GENRE == enum_target) {
		//do thing
		printf("Selected genre\n");
	}
	else if(ID == enum_target) {
		printf("Selected ID\n");
		//do thing
	}
	else {
		printf("Invalid target for select\n");
	}
	return;
}

int* Query_Transform(parse_node* head, const char* query_string) {
	/*This will take the query string from the Request_Parsing function
	and convert it into an int array that can be used in a switch statement
	to determine what the user wants to do. It will return the int array pointer
	to the Request_Parsing function.
	*/
	if( head == NULL || query_string == NULL) {
		printf("Parse tree or tokenized DB is NULL\n");
		return NULL;
	}

	int* int_array = malloc(10 * sizeof(int)); //arbitrary size for now	

	for (int i = 0; i < 10; i++) {
		int_array[i] = -1; //initialize to -1
	}

	if (int_array == NULL) {
		printf("Memory allocation failed for int_array\n");
		return NULL;
	}	

	char query_string2[256];
	memcpy_s(query_string2, sizeof(query_string2), query_string, strlen(query_string) + 1);

	char* context = NULL;
	char* token = strtok_s(query_string2, "%", &context);

	int tracker = 0;
	while(token != NULL) {
		
		//Will have to realloc if more than 10 tokens are found

		int_array[tracker] = Does_Command_Exist(head, token);
		token = strtok_s(NULL, "%", &context);
		
		//Special case for the titles name input
		if (tracker == 4) {

			errno_t result = strcpy_s(title_str, sizeof(title_str), token);
			if (result != 0) {
				free(title_str);
				return NULL;
			}
		}

		tracker += 1;

	}
	return int_array;
}

void Request_Parsing(parse_node* head, const char* db_request) {
	/*this will call the Query_Transform function to get the int array
	then it will use that array accross a large switch statement to
	do the required operations. It will return a Response struct that
	will then be transformed into JSON and sent back to the requester
	*/
	int* parsed_array = Query_Transform(head, db_request);

	if (parsed_array == NULL) {
		printf("Parsed array is NULL\n");
		return; //return object will go here
	}

	//Stage one 
	switch (parsed_array[0]) {
	case SELECT:
		//do thing
		switch (parsed_array[1]) {
			case TITLE:
				switch(parsed_array[2]) {
					case WHERE:
						switch (parsed_array[3]) {
						case TITLE:
							switch (parsed_array[4]) {
								case EQUALS:
									//FOR MOST QUERIES AT THIS TIME, THIS IS WHERE IT WILL END
									//parsed_array[5] will be the string to search for
									printf("Got to equals with string: %s\n", title_str);

									break;
								case LESSTHAN:
									//do thing
									break;
								case GREATERTHAN:
									//do thing
									break;
								case LIKE:
									//do thing
									break;
								case ORDERBY:
									//do thing
									break;
								case GROUPBY:
									//do thing
									break;
								case ASCENDING:
									//do thing
									break;
								case DESCENDING:
									//do thing
									break;
								/*=======DEFAULTS//ERRORS============*/
								case -1:
									printf("Invalid command in query\n");
									break;
								default:
									printf("Unhandled command in query\n");
									break;
							
							}

								break;
							case DESCRIPTION:
								//do thing
								break;
							case GENRE:
								//do thing
								break;
							/*=======DEFAULTS//ERRORS============*/
							case -1:
								printf("Invalid command in query\n");
								break;
							default:
								printf("Unhandled command in query\n");
								break;
						}

						break;
					case ALL:
						//do thing
						break;
					/*=======DEFAULTS//ERRORS============*/
					case -1:
						printf("Invalid command in query\n");
						break;
					default:
						printf("Unhandled command in query\n");
						break;
				}

				//do thing
				break;
			case DESCRIPTION:
				//do thing
				break;
			case GENRE:
				//do thing
				break;
			/*=======DEFAULTS//ERRORS============*/
			case -1:
				printf("Invalid command in query\n");
				break;
			default:
				printf("Unhandled command in query\n");
				break;
		}

		break;
	case CHANGE:
		//do thing
		break;
	case REMOVE:
		//do thing
		break;
	case SEARCH:
		//do thing
		break;
	case -1:
		/*=======DEFAULTS//ERRORS============*/
		printf("Invalid command in query\n");
		break;
	default:
		printf("Unhandled command in query\n");
		break;
	}


}

//to be called when a video stream is requested
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