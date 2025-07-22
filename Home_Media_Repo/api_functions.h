#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include "master.h"
#include "data_structures.h"


cJSON* Get_All_Media(MediaData** hash_table, char* title, size_t array_size);


cJSON* Input_String_Parsing(MediaData** hash_table, char* user_input, size_t array_size);

#endif // !API_FUNCTIONS_H