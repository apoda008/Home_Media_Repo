#pragma once
#ifndef FOLDER_MANAGEMENT_H
#define FOLDER_MANAGEMENT_H

#include "master.h"

bool BrowseForFolder(Master_Directory* ptr, int create_or_move);

void Create_Folders(Master_Directory* global_ptr);

void MoveMediaToMaster(TCHAR* path, Master_Directory* global_ptr);

TCHAR* Parse_Helper(TCHAR title);

int Media_Files_Into_Table(Master_Directory* global_ptr, DatabaseStructure* Database);

void FolderExecution(Master_Directory* global_ptr);

void Copy_To_Mp4(TCHAR* path, Master_Directory* global_ptr);


#endif // !FOLDER_MANAGEMENT_H
