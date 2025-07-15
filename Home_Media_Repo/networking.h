#ifndef NETWORKING_H
#define NETWORKING_H

#include "master.h"
#include "folder_management.h"

struct Memory {
    char* string;
    size_t size;
};


size_t write_chunk(void* data, size_t size, size_t nmemb, void* user_data);

void information_Request(TCHAR* title, Master_Directory* global_ptr);



//Establishes pipe to server
bool pipe_to_server();

#endif // !NETWORKING_H
#pragma once
