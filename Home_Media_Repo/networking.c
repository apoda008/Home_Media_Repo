#include "networking.h"

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
		}
	}//end of JSON parsing

	curl_easy_cleanup(hnd);
	free(response.string);
	free(parsed_movie_title);
	parsed_movie_title = NULL; //removes dangling pointer
	global_ptr->tmdb_limiter++; //increment the limiter for the next call
	return 0;
}//end of information_request 


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