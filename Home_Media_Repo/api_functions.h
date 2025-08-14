#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include "master.h"
#include "data_structures.h"

typedef struct {
	struct Node* L_one;
	struct Node* L_two;
	struct Node* L_three;

} Trie;


enum WhatCommands {
	SELECT = 0,
	CHANGE = 1,
	REMOVE = 3,
	SEARCH = 4,
	TITLE = 5,
	DESCRIPTION = 6,
	GENRE = 7,
	ALL = 8
};

enum DataBaseChoices {
	MOVIE = 0,
	SERIES = 1
};

//void VideoTest(SOCKET client_socket);

cJSON* Get_All_Media(MediaData** hash_table, char* title, size_t array_size);


cJSON* Input_String_Parsing(MediaData** hash_table, char* user_input, size_t array_size);



#endif // !API_FUNCTIONS_H