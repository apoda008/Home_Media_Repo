#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include "master.h"
#include "data_structures.h"
#include "Parse.h"

//void VideoTest(SOCKET client_socket);
// 
//transforms the parse tree into an int array for easier processing
int* Query_Transform(parse_node* head, char* query_string);

cJSON* Request_Parsing(const DatabaseStructure* database_table, parse_node* head, const char* db_request);

int Stream_Video(SOCKET client_socket, MediaData** hash_table, size_t array_size, char* title);

//returns a MovieTable or SeriesTable based on the target parameter with the requested information
MovieTable* Select_Movies(const MovieTable* movies_table, int* switch_array);

void* Stream_Video_V2(int vid_rang, char* request, const DatabaseStructure* database_table, parse_node* head);


#endif // !API_FUNCTIONS_H