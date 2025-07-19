#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include "master.h"
#include "data_structures.h"

cJSON* get_media(MediaData** hash_table, char* title, size_t array_size);

cJSON* input_string_parsing(MediaData** hash_table, char* user_input, size_t array_size);

#endif // !API_FUNCTIONS_H