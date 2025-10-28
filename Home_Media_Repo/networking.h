#ifndef NETWORKING_H
#define NETWORKING_H

#include "master.h"
#include "folder_management.h"
//#include "data_structures.h"
#include "api_functions.h"

struct Memory {
    char* string;
    size_t size;
};

//respose structure for api calls
typedef struct Response {
    int status_code;
    char* message;
    cJSON* data;
} Response;

typedef struct Request {
    char* authorization[64];
    bool stream_or_request; //true = stream, false = request
    __int64 video_position;
	__int64 request_vid_size;
	int req_string_length;
    char* request;
} Request;


void ConvertTCHARtoUTF8(const TCHAR* input, char* output, size_t outputSize);

size_t write_chunk(void* data, size_t size, size_t nmemb, void* user_data);

void media_write(cJSON* title, cJSON* description, cJSON* id, cJSON* genre_ids, cJSON* media_type, TCHAR* movie_dir, Master_Directory* global_ptr);

void information_Request(TCHAR* title, Master_Directory* global_ptr, TCHAR* dir_title);

void Api_Connection(DatabaseStructure* db_table, parse_node* head);


//Establishes pipe to server
bool pipe_to_server();

#endif // !NETWORKING_H
#pragma once
