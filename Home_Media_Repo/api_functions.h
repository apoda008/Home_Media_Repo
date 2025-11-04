#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include "master.h"
//#include "data_structures.h"
#include "Parse.h"

#define PKT_SIZE (1024u * 1024u)

//void VideoTest(SOCKET client_socket);

typedef struct VideoStream {
	__int64 vid_position; //starting position of file read
	__int64 vid_size; //size to be read
	char* request; //SQL request to get video file
	const DatabaseStructure* database_table; //pointer to the database structure
	const parse_node* head; //pointer to the parse tree
	const SOCKET client_socket; //socket to stream to
} VideoStream;

//transforms the parse tree into an int array for execution 
int* Query_Transform(parse_node* head, char* query_string);

cJSON* Request_Parsing(const DatabaseStructure* database_table, parse_node* head, const char* db_request);

//returns a MovieTable or SeriesTable based on the target parameter with the requested information
MovieTable* Select_Movies(const MovieTable* movies_table, int* switch_array);

int Stream_Video_V2(const VideoStream vid_req_obj);


#endif // !API_FUNCTIONS_H