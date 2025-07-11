#pragma once
#ifndef FOLDER_MANAGEMENT_H
#define FOLDER_MANAGEMENT_H

#include "master.h"

struct Master_Directory {
    TCHAR master_folder[MAX_PATH];
    TCHAR path_to_media_import[MAX_PATH];
    TCHAR movie_bin_path[MAX_PATH];
    TCHAR genre_path[MAX_PATH];
    TCHAR series_bin_path[MAX_PATH];

};

void BrowseForFolder();

#endif // !FOLDER_MANAGEMENT_H
