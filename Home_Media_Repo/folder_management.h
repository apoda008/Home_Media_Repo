#pragma once
#ifndef FOLDER_MANAGEMENT_H
#define FOLDER_MANAGEMENT_H

#include "master.h"

//typedef struct Master_Directory {
//    TCHAR master_folder[MAX_PATH];
//   
//    TCHAR genre_path[MAX_PATH];
//    TCHAR temp_folder_path[MAX_PATH];
//    TCHAR path_to_media[MAX_PATH];
//    
//    TCHAR path_to_media_for_import[MAX_PATH];
//
//    TCHAR movie_bin_path[MAX_PATH];
//    TCHAR series_bin_path[MAX_PATH];
//    size_t tmdb_limiter;
//} Master_Directory;

bool BrowseForFolder(Master_Directory* ptr, int create_or_move);

void Create_Folders(Master_Directory* global_ptr);

void MoveMediaToMaster(TCHAR* path, Master_Directory* global_ptr);

TCHAR* Parse_Helper(TCHAR title);

int File_Search_Parse(Master_Directory* global_ptr);

void FolderExecution(Master_Directory* global_ptr);


#endif // !FOLDER_MANAGEMENT_H
