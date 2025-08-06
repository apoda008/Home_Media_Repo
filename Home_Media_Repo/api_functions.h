#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include "master.h"
#include "data_structures.h"

enum WhatCommands {
	SELECT = 0,
	ADD = 1,
	CHANGE = 2,
	DELETE_ITEM = 3
};

enum WhereCommands {
	TITLE = 0,
	DESCRIPTION = 1,
	GENRE = 2,
	ALL = 3
};

enum WhichCommands {

};





enum DataBaseChoices {
	MOVIE = 0,
	SERIES = 1
};

//void VideoTest(SOCKET client_socket);

cJSON* Get_All_Media(MediaData** hash_table, char* title, size_t array_size);


cJSON* Input_String_Parsing(MediaData** hash_table, char* user_input, size_t array_size);



#endif // !API_FUNCTIONS_H