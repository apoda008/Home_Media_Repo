#pragma once
#ifndef MASTER_H
#define MASTER_H

//MASTER HEADER FOR ALL CLASS LIB
//Exceptions:

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>
#include <ctype.h>
#include <curl/curl.h>
#include "cJSON.h"

#include <shlobj.h>
//#include "program_functions.h"
typedef struct Master_Directory {
    TCHAR master_folder[MAX_PATH];

    TCHAR genre_path[MAX_PATH];
    TCHAR temp_folder_path[MAX_PATH];
    TCHAR path_to_media[MAX_PATH];

    TCHAR path_to_media_for_import[MAX_PATH];

    TCHAR movie_bin_path[MAX_PATH];
    TCHAR series_bin_path[MAX_PATH];
    size_t tmdb_limiter;
    size_t num_of_files;
} Master_Directory;


#endif //END