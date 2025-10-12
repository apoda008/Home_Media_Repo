#include "api_functions.h"

static char title_str[256];

cJSON* Parse_To_JSON(MovieTable* result_table) {
	if (result_table == NULL) {
		return NULL;
	}
	cJSON* json_array = cJSON_CreateArray();
	if (json_array == NULL) {
		printf("Failed to create JSON array\n");
		return NULL;
	}
	
	cJSON* movie_obj = cJSON_CreateObject();
	if (movie_obj == NULL) {
		printf("Failed to create JSON object for movie\n");
		cJSON_Delete(json_array);
		return NULL;
	}

	printf("Number of elements in result_table: %d\n", result_table->num_elements_MV);
	
	if (result_table->num_elements_MV == 1) {
		cJSON_AddNumberToObject(movie_obj, "id", *(result_table->id));
		cJSON_AddStringToObject(movie_obj, "title", *(result_table->title));
		cJSON_AddStringToObject(movie_obj, "description", *(result_table->description));
		// Note: dir_position is TCHAR, conversion may be needed
		//cJSON_AddStringToObject(movie_obj, "dir_position", *(result_table->dir_position));
		cJSON_AddNumberToObject(movie_obj, "video_size", *(result_table->video_size));
		cJSON_AddItemToArray(json_array, movie_obj);
		return json_array;
	}
	cJSON_Delete(movie_obj); // Delete the single object as we will create new ones in the loop
	
	for (int i = 0; i < result_table->num_elements_MV; i++) {	
		//THIS NEEDS SO MUCH ERROR CHECKING. LOOK FOR INSTANCES WHERE IT IS ONLY RETURNING A SINGLE ITEM OR NULL
		
		cJSON* movie_obj = cJSON_CreateObject();

		if (result_table->id != NULL) {
			cJSON_AddNumberToObject(movie_obj, "id", result_table->id[i]);
		} 
		//printf("1\n");
		if (result_table->title != NULL) {
			cJSON_AddStringToObject(movie_obj, "title", result_table->title[i]);
		}
		//printf("2\n");
		//I don't plan on dir position being used so commenting out for now
		if (result_table->description != NULL) {
			cJSON_AddStringToObject(movie_obj, "description", result_table->description[i]);
		}
		//printf("3\n");
		//I don't plan on dir position being used so commenting out for now
		if (result_table->dir_position != NULL) {
			// Note: dir_position is TCHAR, conversion may be needed
			//cJSON_AddStringToObject(movie_obj, "dir_position", result_table->dir_position[i]);
		}
		//printf("4\n");	
		if (result_table->video_size != NULL) {
			cJSON_AddNumberToObject(movie_obj, "video_size", result_table->video_size[i]);
		}
		//printf("5\n");
		
		
		//cJSON_AddNumberToObject(movie_obj, "id", result_table->id[i]);
		//cJSON_AddStringToObject(movie_obj, "title", result_table->title[i]);
		//cJSON_AddStringToObject(movie_obj, "description", result_table->description[i]);
		//// Note: dir_position is TCHAR, conversion may be needed
		//cJSON_AddStringToObject(movie_obj, "dir_position", result_table->dir_position[i]);
		//cJSON_AddNumberToObject(movie_obj, "video_size", result_table->video_size[i]);
		cJSON_AddItemToArray(json_array, movie_obj);

	}
	return json_array;
}

int Table_Look_Up_Title(const MovieTable* movies_table, const char* title) {
	if (movies_table == NULL) {
		return -2; //error
	}

	int table_pos = -1;
	if (title != NULL) {
		//if the table is organized by title, a binary search can be implemented here

		//since its currently not organized, a linear search will be used
		for (int i = 0; i < movies_table->num_elements_MV; i++) {
			if (strcmp(movies_table->title[i], title_str) == 0) {
				table_pos = i;
				return table_pos; //return object will go here
			}
		}
		return table_pos; //not found
	}
	return -2; //title was NULL
}

MovieTable* Select_Movies(const MovieTable* movies_table, int* int_array) {
	
	if (movies_table == NULL || int_array == NULL) {
		printf("Movie table or int array is NULL\n");
		return;
	}

	MovieTable* result_table = malloc(sizeof(MovieTable));
	if (result_table == NULL) {
		printf("Memory allocation failed for result_table\n");
		return;
	}

	//initialize result table
	result_table->id = NULL;
	result_table->title = NULL;
	result_table->description = NULL;
	result_table->dir_position = NULL;
	result_table->video_size = NULL;
	result_table->num_elements_MV = 0;
	
	switch (int_array[3]) {
	case MOVIES:
		switch (int_array[1])
		{
		case ALL:

			if (int_array[4] != -1) {

				switch (int_array[5])
				{ //WHERE
				case TITLE:

					switch (int_array[6])
					{
					case EQUALS:
					{
						//return all data for a specific title
						int i = Table_Look_Up_Title(movies_table, title_str);
						result_table->id = &movies_table->id[i];
						result_table->video_size = &movies_table->video_size[i];
						result_table->dir_position = &movies_table->dir_position[i];
						result_table->title = &movies_table->title[i];
						result_table->description = &movies_table->description[i];
						result_table->num_elements_MV = 1;
						//I don't plan on dir position being used so commenting out for now
						//result_table->dir_position = &movies_table->dir_position[i];
						return result_table; //return object will go here
					} //EQUALS
					break;

					default:
						printf("Invalid target in WHERE clause of SELECT command\n");
						return NULL;
					}

				case ID:

					switch (int_array[6])
					{
					case EQUALS:
					{
						//return all data for a specific ID
						//this function is not entirely safe. Consider making your own str to int function
						int value = atoi(title_str);
						
						if (value < 0 || value >= movies_table->num_elements_MV) {
							printf("ID out of range: %d\n", value);
							printf("Max ID: %d\n", movies_table->num_elements_MV - 1);
							return NULL; //return object will go here
						}
						result_table->num_elements_MV = 1;
						result_table->id = &movies_table->id[value];
						result_table->video_size = &movies_table->video_size[value];
						result_table->dir_position = &movies_table->dir_position[value];
						result_table->title = &movies_table->title[value];
						result_table->description = &movies_table->description[value];

						return result_table;
					}
					break;

					default:
						printf("Invalid target in WHERE clause of SELECT command\n");
						return NULL; //return object will go here
						break;
					}
				
				default:
					printf("Invalid target in WHERE clause of SELECT command\n");
					return NULL;
					break;
				}

				
			}
			else {
				//return entire table
				result_table = movies_table;
				return result_table;
			}
			break;


		case TITLE:
			if (int_array[4] != -1) {
				switch (int_array[5])
				{//WHERE

				case TITLE:

					switch (int_array[6])
					{

					case EQUALS:
						//return title that matches the title string
					{
						int i = Table_Look_Up_Title(movies_table, title_str);
						result_table->title = &movies_table->title[i];
						result_table->num_elements_MV = 1;

						return result_table;
					}
					break;

					default:
						printf("Invalid target in WHERE clause of SELECT command\n");
						return NULL;
						break;
					}

				case ID:
					switch (int_array[6])
					{
					case EQUALS:
						//return title that matches the ID int
					{
						int i = atoi(title_str);
						if( i < 0 || i >= movies_table->num_elements_MV) {
							printf("ID out of range\n");
							return NULL; //return object will go here
						}
						result_table->title = &movies_table->title[i];
						result_table->num_elements_MV = 1;

						return result_table;
					}
					break;
					default:
						printf("Invalid target in WHERE clause of SELECT command\n");
						return NULL;
						break;
					}

				default:
					printf("Invalid target in WHERE clause of SELECT command\n");
					return NULL;
					break;
				}
			}
			else {
				//return all titles in the table
				result_table->title = movies_table->title;
				result_table->num_elements_MV = movies_table->num_elements_MV;
				return result_table;
			}
			break;

		case DESCRIPTION:
			if (int_array[4] != -1) {
				switch (int_array[5]) {
				case TITLE:
					switch (int_array[6]) {
					case EQUALS:
						//return description that matches the title string
					{
						int i = Table_Look_Up_Title(movies_table, title_str);
						result_table->description = &movies_table->description[i];
						result_table->num_elements_MV = 1;
						return result_table;
					}
					break;
					}
					break;
				case ID:
					switch (int_array[6]) {
					case EQUALS:
					{
						//return description that matches the ID int
						int i = atoi(title_str);
						if (i < 0 || i >= movies_table->num_elements_MV) {
							printf("ID out of range\n");
							return NULL; //return object will go here
						}
						result_table->description = &movies_table->description[i];
						result_table->num_elements_MV = 1;

					}
					break;
					}
					break;
				}
			}
			else {
				//return all descriptions in the table
				result_table->description = movies_table->description;
				result_table->num_elements_MV = movies_table->num_elements_MV;
				return result_table;
			}
			break;

		case GENRE:

			if (int_array[4] != -1) {
				switch (int_array[5])
				{

				case TITLE:

					switch (int_array[6])
					{
					case EQUALS:
						//return genres that matches the title string
						break;
					}
					break;
				case ID:
					switch (int_array[6]) {
					case EQUALS:
						//return genres that matches the ID int
						break;
					}
					break;
				}
			}
			else {
				//return all genres in the table
			}
			break;

		case ID:
			if (int_array[4] != -1) {
				switch (int_array[5])
				{
				case TITLE:

					switch (int_array[6])
					{

						case EQUALS:
						//return ID that matches the title string
						{
						int i = Table_Look_Up_Title(movies_table, title_str);
						result_table->id = &movies_table->id[i];
						result_table->num_elements_MV = 1;
						return result_table;
						}
						break;

						default:
							printf("Invalid target in WHERE clause of SELECT command\n");
							return NULL;
							break;
					}
					break;

				case ID:

					switch (int_array[6])
					{

					case EQUALS:
						//return ID that matches the ID int
						{
						int i = atoi(title_str);
						if (i < 0 || i >= movies_table->num_elements_MV) {
							printf("ID out of range\n");
							return NULL; //return object will go here
						}
						result_table->id = &movies_table->id[i];
						result_table->num_elements_MV = 1;
						return result_table;
						}
						break;

					default:
						printf("Invalid target in WHERE clause of SELECT command\n");
						return NULL;
						break;
					}
					break;

				}
			}
			else {
				//return all IDs in the table
				result_table->id = movies_table->id;
				result_table->num_elements_MV = movies_table->num_elements_MV;
				return result_table;
			}
			break;
	
		}//end of switch target
		break; //END OF MOVIES
	

	//IMPLEMENT LATER
	case SERIES:
			//do series thing
			break;
			
	default:
		printf("Invalid source in SELECT command\n");
		return NULL;
		break;
	}

}//end of Select_Movies

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
		if (int_array[tracker] == -1) {
			printf("Parsing error at index: %d\n", tracker);
		}

		token = strtok_s(NULL, "%", &context);
		
		//Special case for the titles name input
		if (tracker == 6) {
			
			//needs null protection
			if(token == NULL) {
				printf("Token is NULL when trying to copy title\n");
				free(int_array);
				return NULL;
			}

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

void Request_Parsing(const DatabaseStructure* database_table, parse_node* head, const char* db_request) {
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

	//test
	//for(int i = 0; i < 10; i++) {
	//	printf("parsed_array[%d] = %d\n", i, parsed_array[i]);
	//}

	//if a remake is not done of the DB structure this will have to be passed in
	MovieTable* movies_table_response = NULL;

	//Stage one 
	switch (parsed_array[0]) {
	case SELECT:
		movies_table_response = Select_Movies(database_table->movies, parsed_array);
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

	if (movies_table_response == NULL) {
		printf("Invalid Input for request\n");
		return; 
	}

	//TEST PURPOSES ONLY DELETE
	//quick print of the returned movie table
	//printf("Returning: \n");
	//Better_Print_Table(movies_table_response);

	//DONT FORGET TO FREE THE MOVIE TABLE IF IT WAS ALLOCATED

	cJSON* json_response = Parse_To_JSON(movies_table_response);

	if (json_response == NULL) {
		printf("Failed to convert response to JSON\n");
		return; //return object will go here
	}

	char* json_string = cJSON_Print(json_response);
	printf("JSON Response:\n%s\n", json_string);

	free(movies_table_response);
	movies_table_response = NULL;

	return;
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