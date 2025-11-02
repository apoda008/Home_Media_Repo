#include "networking.h"
#include "Function_Test.h"



void ConvertTCHARtoUTF8(const TCHAR* input, char* output, size_t outputSize) {
#ifdef UNICODE
	// Convert wide TCHAR (wchar_t*) to UTF-8 char*
	WideCharToMultiByte(CP_UTF8, 0, input, -1, output, (int)outputSize, NULL, NULL);
#else
	// Already char*, just copy
	strncpy_s(output, outputSize, input, _TRUNCATE);
#endif
}

size_t write_chunk(void* contents, size_t size, size_t nmemb, void* userp) {
	
	size_t total_size = size * nmemb;
	struct Memory* mem = (struct Memory*)userp;

	char* ptr = realloc(mem->string, mem->size + total_size + 1);
	if (ptr == NULL) {
		// Allocation failed
		return 0;
	}

	mem->string = ptr;
	memcpy(mem->string + mem->size, contents, total_size);

	if (!mem || !contents) {
		fprintf(stderr, "write_chunk: NULL pointer\n");
		return 0;
	}
	mem->size += total_size;
	mem->string[mem->size] = '\0';  // Null-terminate

	return total_size;
}

void media_write(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, TCHAR* dir_position, Master_Directory* global_ptr) {
	char* first_char_string = title->valuestring;
	TCHAR filename[8] = _T("\\a.bin");

	//because strcat is a bitch
	filename[1] = tolower(first_char_string[0]);

	TCHAR file_buffer[MAX_PATH];
	_tcscpy_s(file_buffer, MAX_PATH, global_ptr->movie_bin_path);
	_tcscat_s(file_buffer, MAX_PATH, filename);

	//DELETE BE AWARE; TEMP IS NOT FREED. 
	//_tprintf(_T("FILE_BUFFER: %s\n"), file_buffer);

	bool is_movie = false;

	if (!strcmp(media_type->valuestring, "movie")) {
		is_movie = true;
	}

	MediaData temp = { 52, "", id->valuedouble, is_movie, {0}, "",_T("") };

	strcpy_s(temp.title, 256, title->valuestring);
	strcpy_s(temp.description, 2000, description->valuestring);
	_tcscpy_s(temp.dir_position_media, MAX_PATH, dir_position);

	cJSON* genre_number;
	int i = 0;
	cJSON_ArrayForEach(genre_number, genre_ids) {
		temp.genre_types[i] = (int)genre_number->valuedouble;
		i++;
	}

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


}//end of media_write

//PROBABLY WONT NEED, WILL MAKE A TABLE_SAVE FUNCTION IN DATABASE STRUCTURES
void Media_WriteV2(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, TCHAR* dir_position, Master_Directory* global_ptr) {
	return;
}

//sourced by themoviedb.org api system
void information_Request(TCHAR* parsed_movie_title, Master_Directory* global_ptr, TCHAR* dir_title) {
	//This is to pass into media_write
	TCHAR dir_position[MAX_PATH];
	_tcscpy_s(dir_position, MAX_PATH, global_ptr->path_to_media);
	_tcscat_s(dir_position, MAX_PATH, _T("\\"));
	_tcscat_s(dir_position, MAX_PATH, dir_title);

	//tmbd request control 
	if (global_ptr->tmdb_limiter >= 40) {
		printf("You have reached the TMDB API limit for this session.\n");
		Sleep(10000);
		global_ptr->tmdb_limiter = 0; //reset the limiter
	}

	//====this is solely to get the key for api call==== 
	//in real implentation this will ask for the users key
	FILE* file = fopen("C:\\Users\\dan_a\\Desktop\\key.txt", "r");
	if (file == NULL) {
		perror("TEMP FILE FAILED\n");
	}
	char authorization[267] = "";

	fgets(authorization, 267, file);
	fclose(file);
	//==========================================================

	CURL* hnd = curl_easy_init();

	char* utf8_movie[MAX_PATH];
	ConvertTCHARtoUTF8(parsed_movie_title, utf8_movie, sizeof(utf8_movie));

	char search_buffer[MAX_PATH];
	snprintf(search_buffer, sizeof(search_buffer), "https://api.themoviedb.org/3/search/multi?query=%s&include_adult=false&language=en-US", utf8_movie);

	if (!hnd) {
		fprintf(stderr, "Error with curl initialization \n");
		return 0;
	}

	//object for response
	struct Memory response;
	response.string = malloc(1);
	response.size = 0;

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_chunk); //needs to be nulled for stdout
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void*)&response); //needs to nulled for stdout

	//DELETE strictly for viewing what is being fully returned in the JSON, serves no function to the program overall
	//curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);

	curl_easy_setopt(hnd, CURLOPT_URL, search_buffer);

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "accept: application/json");
	headers = curl_slist_append(headers, authorization);
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	CURLcode ret = curl_easy_perform(hnd);

	if (ret != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
	}
	else {
		//JSON parsing
		cJSON* tmdb_json = cJSON_Parse(response.string);
		if (!tmdb_json) {
			fprintf(stderr, "Error parsing JSON\n");
			return 0;
		}
		else {

			cJSON* results = cJSON_GetObjectItemCaseSensitive(tmdb_json, "results");

			if (cJSON_IsArray) {

				cJSON* movie = NULL;

				//grab first response and break out of loop
				cJSON_ArrayForEach(movie, results) {
					//DELETE
					//char* json_str2 = cJSON_Print(movie);
					//printf("THING: %s\n", json_str2);

					cJSON* title = cJSON_GetArrayItem(movie, 3);
					cJSON* description = cJSON_GetObjectItemCaseSensitive(movie, "overview");
					cJSON* id = cJSON_GetObjectItemCaseSensitive(movie, "id");
					cJSON* genre_ids = cJSON_GetObjectItemCaseSensitive(movie, "genre_ids");
					cJSON* media_type = cJSON_GetObjectItemCaseSensitive(movie, "media_type");

					media_write(title, description, id, genre_ids, media_type, dir_position, global_ptr);
					break;

					//media_write(title, description, id, genre_ids, media_type, dir_position, create_folder_location);

					/*
						NOTE: FOR INSTANCES WHERE THE MOVIE INFORMATION RETURNED WAS INCORRECT IT
						WILL HAVE TO BE FIXED BY THE USER AT A LATER DATE USING THE TO-BE-IMPLEMENTED
						DATABASE FUNCTIONS
					*/
				}
			}
			//Frees JSON after use
			cJSON_Delete(tmdb_json);
		}
	}//end of JSON parsing

	curl_easy_cleanup(hnd);
	free(response.string);
	free(parsed_movie_title);
	parsed_movie_title = NULL; //removes dangling pointer
	global_ptr->tmdb_limiter++; //increment the limiter for the next call

	
	return 0;
}//end of information_request 

//===============================================NEW STUFF TO BE MOVED LATER====================================================

//MAY CAUSE CONFLICTION ON COMPLIATION
//gets the total byte size of the movie 
__int64 GetVideoSize(TCHAR* movie_path) {
	_tprintf(_T("Getting video size for: %s\n"), movie_path);

	FILE* video_file = _tfopen(movie_path, _T("rb"));
	if (video_file == NULL) {
		_tperror(_T("Failed to open video file"));
		return -1;
	}

	// Use 64-bit seek for large files
	if (_fseeki64(video_file, 0, SEEK_END) != 0) {
		_tperror(_T("Error seeking to end of video file"));
		fclose(video_file);
		return -1;
	}

	__int64 result = _ftelli64(video_file);
	if (result == -1) {
		_tperror(_T("Error getting file size"));
		fclose(video_file);
		return -1;
	}

	//DELETE
	//_tprintf(_T("Size: %I64d bytes\n"), result);

	fclose(video_file);
	return result;
}



void From_Json_To_Table(cJSON* tmdb_json, DatabaseStructure* Database, Master_Directory* global_ptr, TCHAR* dir_title) {
	
	//This is a set up to be moved into the table for the media path
	TCHAR dir_position[MAX_PATH];
	_tcscpy_s(dir_position, MAX_PATH, global_ptr->path_to_media);
	_tcscat_s(dir_position, MAX_PATH, _T("\\"));
	_tcscat_s(dir_position, MAX_PATH, dir_title);
	


	if (!tmdb_json) {
		fprintf(stderr, "Error parsing JSON\n");
		return 0;
	}
	else {

		cJSON* results = cJSON_GetObjectItemCaseSensitive(tmdb_json, "results");

		if (cJSON_IsArray) {

			cJSON* movie = NULL;

			//grab first response and break out of loop. Wont always be the correct response 
			//will need users to correct later through DB api
			cJSON_ArrayForEach(movie, results) {
				//DELETE
				//char* json_str2 = cJSON_Print(movie);
				//printf("THING: %s\n", json_str2);

				cJSON* title = cJSON_GetArrayItem(movie, 3);
				cJSON* description = cJSON_GetObjectItemCaseSensitive(movie, "overview");
				cJSON* id = cJSON_GetObjectItemCaseSensitive(movie, "id");
				cJSON* genre_ids = cJSON_GetObjectItemCaseSensitive(movie, "genre_ids");
				cJSON* media_type = cJSON_GetObjectItemCaseSensitive(movie, "media_type");
				__int64 video_size = GetVideoSize(dir_position);
				
				//moves data directly into table 
				Insert_Movie(Database, title->valuestring, description->valuestring, dir_position, video_size);

				break;
			}
		}
		//Frees JSON after use
		cJSON_Delete(tmdb_json);
	}

}

//============================================================================================================

//updated func to try and get info from TMDB and return the JSON that TMDB gives you
cJSON* Information_RequestV2(TCHAR* parsed_movie_title) {

	//====this is solely to get the key for api call============= 
	FILE* file = fopen("C:\\Users\\dan_a\\Desktop\\key.txt", "r");
	if (file == NULL) {
		perror("TEMP FILE FAILED\n");
		//return NULL;
	}
	char authorization[267] = "";

	if (fgets(authorization, 267, file) == NULL) {
		perror("Failed to get API Key\n");
		return NULL;
	};
	
	fclose(file);
	//==========================================================

	CURL* hnd = curl_easy_init();

	char* utf8_movie[MAX_PATH];
	ConvertTCHARtoUTF8(parsed_movie_title, utf8_movie, sizeof(utf8_movie));

	char search_buffer[MAX_PATH];
	snprintf(search_buffer, sizeof(search_buffer), "https://api.themoviedb.org/3/search/multi?query=%s&include_adult=false&language=en-US", utf8_movie);

	if (!hnd) {
		fprintf(stderr, "Error with curl initialization \n");
		return NULL;
	}

	//object for response
	struct Memory response;
	response.string = malloc(1);
	response.size = 0;

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_chunk); //needs to be nulled for stdout
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void*)&response); //needs to nulled for stdout

	//DELETE strictly for viewing what is being fully returned in the JSON, serves no function to the program overall
	//curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);

	curl_easy_setopt(hnd, CURLOPT_URL, search_buffer);

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "accept: application/json");
	headers = curl_slist_append(headers, authorization);
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	CURLcode ret = curl_easy_perform(hnd);

	if (ret != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));

	}
	else {
		
		cJSON* tmdb_json_response = cJSON_Parse(response.string);
		if (!tmdb_json_response) {
			fprintf(stderr, "Error parsing JSON\n");
			return NULL;
		}
		curl_easy_cleanup(hnd);
		free(response.string);
		parsed_movie_title = NULL; //removes dangling pointer
		return tmdb_json_response;
	}

	//clean up and close on fail
	curl_easy_cleanup(hnd);
	free(response.string);
	parsed_movie_title = NULL; //removes dangling pointer 
	return NULL;
}


bool IpAddress_Validation(const char* ip_address) {
	bool end = false;
	char* context;
	char* token = strtok_s(ip_address, ".", &context);

	if (strcmp(token, "192") == 0){
		token = strtok_s(NULL, ".", &context);
		if (strcmp(token, "168") == 0) {
			token = strtok_s(NULL, ".", &context);
			end = true; //Valid Network Address
		}
		else { end = false; } //NonValid Network address
	}
	else { end = false; }// NonValid Network address

	return end;
}

//====================================================================================
////This is the main API connection function that will be used to connect to the server
void Api_Connection(DatabaseStructure* db_table, parse_node* head) {

	//Start of connection
	WSADATA wsaData;
	SOCKET database_socket, client_socket;
	struct sockaddr_in database_addr, client_addr;
	int client_len = sizeof(client_addr);
	char buffer[4096] = { 0 };
	char client_ip[INET6_ADDRSTRLEN] = { 0 };

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	database_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (database_socket == INVALID_SOCKET) {
		printf("Socket failed: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	database_addr.sin_family = AF_INET;
	database_addr.sin_port = htons(5001);
	//database_addr.sin_addr.s_addr = INADDR_ANY;




	//this will need to be adjusted for flexibilty;
	//needs to dynamically grab its own network address
	//Other issues: how would outside programs know my porting #
	//without it they cannot talk to this server and it needs to 
	//be able to be accessed by other devices on the network
	//without hardcoding the port.
	inet_pton(AF_INET, "192.168.4.81", &database_addr.sin_addr);

	if (bind(database_socket, (SOCKADDR*)&database_addr, sizeof(database_addr)) == SOCKET_ERROR) {
		printf("Bind Failed: %d\n", WSAGetLastError());
		closesocket(database_socket);
		return 1;
	}


	bool close = false;
	while (1) {
		//this function will need to be adjusted to allow for multiple connections

		listen(database_socket, SOMAXCONN);

		printf("Database Listening.. \n\n");

		client_socket = accept(database_socket, (SOCKADDR*)&client_addr, &client_len);

		if (client_socket == INVALID_SOCKET) {
			printf("Accept failed: %d \n", WSAGetLastError());
			closesocket(database_socket);
			WSACleanup();
			return 1;
		}

		inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	
		//This will become a log
		printf("Client IP: %s, Port: %d\n", client_ip, ntohs(client_addr.sin_port));
	
		if (IpAddress_Validation(client_ip) == false) {
			//break connection:
			printf("Invalid address\n");
			closesocket(client_socket);
		}
			
			int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
			if (bytes_received > 0) {
				
				//might want to wait on this one
				
				Request req_struct;

				//copies the first 64 bytes into the authorization field
				memcpy_s(req_struct.authorization, 64, buffer, 64);
				req_struct.authorization[63] = '\0'; //null terminate
				printf("Authorization received: %s\n", req_struct.authorization);

				//copies the 65th byte into the stream_or_request field
				memcpy_s(&req_struct.stream_or_request, sizeof(bool), buffer + 64, sizeof(bool));
				printf("Stream or Request flag received: %d\n", req_struct.stream_or_request);

				//copies the next 8 bytes into the video_position field
				memcpy_s(&req_struct.video_position, sizeof(__int64), buffer + 65, sizeof(__int64));
				req_struct.video_position = _byteswap_uint64(req_struct.video_position); //convert from network to host byte order
				printf("Video position received: %lld\n", req_struct.video_position);

				//copies the next 8 bytes into the request_length field
				memcpy_s(&req_struct.request_vid_size, sizeof(__int64), buffer + 73, sizeof(__int64));
				req_struct.request_vid_size = _byteswap_uint64(req_struct.request_vid_size); //convert from network to host byte order
				printf("Request length received: %lld\n", req_struct.request_vid_size);

				//copies 4 bytes into req_length
				int req_length = 0;
				memcpy_s(&req_struct.req_string_length, sizeof(int), buffer + 81, sizeof(int));
				req_length = _byteswap_ulong(req_struct.req_string_length); //convert from network to host byte order
				printf("Request string length received: %d\n", req_length);

				//copies the rest into the request field
				

				req_struct.request = (char*)malloc(req_length + 1); //allocate memory for request
				if(req_struct.request == NULL) {
					printf("Memory allocation for request failed\n");
					//send error response to client
					const char* error_response = "{\"status_code\":500,\"message\":\"Internal Server Error\",\"data\":null}";
					send(client_socket, error_response, strlen(error_response), 0);
					break; // break and wait for new client
				}

				memcpy_s(req_struct.request, 256, buffer + 85, req_length);
				req_struct.request[req_length] = '\0'; //null terminate
				printf("Request String received: %s\n", req_struct.request);

				if(!req_struct.stream_or_request) {
					//REQUEST PROCESSING==========================================
					printf("Request processing selected.\n");
				
					//NEW REQUEST PROCESSING WAY
					cJSON* req = Request_Parsing(db_table, head, req_struct.request);
				
					free(req_struct.request); //free the allocated memory for request

					if(req == NULL) {
						printf("Request parsing failed or returned NULL\n");
						//send error response to client
						const char* error_response = "{\"status_code\":400,\"message\":\"Bad Request\",\"data\":null}";
						send(client_socket, error_response, strlen(error_response), 0);
						break; // break and wait for new client
					}
				
					//DELETE OR LOG
					char* j_print = cJSON_Print(req);
					printf("sending (as JSON) %s\n", j_print);

					send(client_socket, j_print, strlen(j_print), 0);
					printf("Response sent to client.\n");
					cJSON_Delete(req);
					//break; //break to wait for new client
				}
				else {
					//STREAM PROCESSING==========================================
					printf("Stream processing selected.\n");
					
					VideoStream vid_obj = { req_struct.video_position, req_struct.request_vid_size, req_struct.request, db_table, head, client_socket };

					Stream_Video_V2(vid_obj);
				}


			} else if (bytes_received == 0) {
				printf("Connection closed by client.\n");
				
			}
			else {
				printf("recv failed: %d\n", WSAGetLastError());
				
			}

		//this will be the var to use to shut down listening and close all connections
		printf("Closing connection with client...\n");
		close = true;
		closesocket(client_socket);
		
	}
	closesocket(database_socket);
	WSACleanup();
		
	return 0;

}

/*============NOTES================ 
So to make it so that the server can talk over a network that doesnt know it exists
is to set server.sin_addr.s_addr = INADDR_ANY, 

ERROR handling here is fucked. It will crash the program if a bad request is sent 
more specifically, the switch statements are not catching it and returning properly.

*/







//ignore this for now
bool pipe_to_server() 
{	
	const char* pipeNamed = "\\\\.\\pipe\\MediaRepoPipe";

	HANDLE pipe = CreateNamedPipeA(
		pipeNamed,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,					//max instances
		4096, 4096,			//Output and input buffer size
		0,					//default time out 
		NULL				//default security
	);

	if (pipe == INVALID_HANDLE_VALUE) 
	{
		printf("CreateNamedPipe failed: %lu\n", GetLastError());
	}

	printf("Waiting for client to connect....\n");

	BOOL connected = ConnectNamedPipe(pipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

	if (connected) {
		
		char buffer[128];
		DWORD bytesRead;
		ReadFile(pipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
		buffer[bytesRead] = '\0';
		printf("Received from client: %s\n", buffer);

		const char* response = "Hello from server!";
		DWORD bytesWritten;
		WriteFile(pipe, response, (DWORD)strlen(response), &bytesWritten, NULL);
	}

	CloseHandle(pipe);
	return true;
}