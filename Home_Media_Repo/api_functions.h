#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include "master.h"
#include "data_structures.h"
#include "Parse.h"


//enum DataBaseChoices {
//	//MOVIE = 0,
//	//SERIES = 1
//};

//void VideoTest(SOCKET client_socket);

typedef struct Response {
	int status_code;
	char* message;
	cJSON* data;
} Response;

int* Query_Transform(parse_node* head, char* query_string);

void Request_Parsing(parse_node* head, const char* db_request);

int Stream_Video(SOCKET client_socket, MediaData** hash_table, size_t array_size, char* title);

#endif // !API_FUNCTIONS_H