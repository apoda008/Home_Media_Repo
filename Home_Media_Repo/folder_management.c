#include "folder_management.h"


void BrowseForFolder(Master_Directory* ptr, int create_or_move) {
    
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Select a folder");
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (pidl != NULL)
    {
        if (create_or_move == 0) {
        
            char path[MAX_PATH];
            if (SHGetPathFromIDList(pidl, path))
            {
                _tprintf(_T("Folder selected: %s\n"), path);
                _tcscpy_s(ptr->master_folder, MAX_PATH, path);
            }
            CoTaskMemFree(pidl);
        }
        else 
        {
            char path[MAX_PATH];
            if (SHGetPathFromIDList(pidl, path))
            {
                _tprintf(_T("Folder selected: %s\n"), path);
                _tcscpy_s(ptr->path_to_media_for_import, MAX_PATH, path);
            }
            CoTaskMemFree(pidl);
        }
    }
}

void Create_Folders(Master_Directory* global_ptr) {
    //Creates all the folders the program will use
    
    //updates the master path
    _tcscat_s(global_ptr->master_folder, MAX_PATH, _T("\\Media_Repository"));
    
    if (CreateDirectory(global_ptr->master_folder, NULL)) {
        _tprintf(TEXT("\nFolder created successfully: %s\n"), global_ptr->master_folder);
       
    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            _tprintf(TEXT("\nDatabase folder already exists: %s\n"), global_ptr->master_folder);
          
        }
        else {
            _tprintf(TEXT("\nfailed: %s\n"), global_ptr->master_folder);
         
        }
    }

    //Movie media folder where the physical media files will be held
    
    //sets the media path and concats Media Movies to it for creation
    _tcscpy_s(global_ptr->path_to_media, MAX_PATH, global_ptr->master_folder);
    _tcscat_s(global_ptr->path_to_media, MAX_PATH, _T("\\Media_Movies"));
    

    if (CreateDirectory(global_ptr->path_to_media, NULL)) {
        _tprintf(TEXT("\nFolder created successfully: %s\n"), global_ptr->path_to_media);
       
    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            _tprintf(TEXT("\nDatabase folder already exists: %s\n"), global_ptr->path_to_media);
          
        }
        else {
            _tprintf(TEXT("\nfailed: %s\n"), global_ptr->path_to_media);
            
        }
    }


    //TODO:
    //MORE FOLDER CREATION
    //Bin folders
    //Series folders 
}

void MoveMediaToMaster(TCHAR* path) {
   
    TCHAR copy_path[MAX_PATH];

    //sets the * for a search through all files in the directory
    _tcscpy_s(copy_path, MAX_PATH, path);
    _tcscat_s(copy_path, MAX_PATH, _T("\\*"));

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(copy_path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Error handle value. Error: \n"), GetLastError());
    }
    else {
        do {
            
            //Skip "." ".." file names
            if ( (strcmp(findFileData.cFileName, ".") == 0) || (strcmp(findFileData.cFileName, "..") == 0 ) ) {
                continue;
            }
            
            //check if it's a directory or a file
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                _tprintf(_T("[DIR]: %s\n"), findFileData.cFileName);
                
                //Set up the next directory
                TCHAR next_dir[MAX_PATH];

                _tcscat_s(next_dir, MAX_PATH, path);
                _tcscat_s(next_dir, MAX_PATH, _T("\\"));
                _tcscat_s(next_dir, MAX_PATH, findFileData.cFileName);
                MoveMediaToMaster(next_dir);
            }
            else {
                _tprintf(_T("[FILE]: %s\n"), findFileData.cFileName);
                //This is when we would move them all to the primary file
            }
            
        } while (FindNextFile(hFind, &findFileData) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES) {
            printf("FindNextFile Failed (%d)\n", GetLastError());
        }

        FindClose(hFind);
    }

   

}

void FolderExecution(Master_Directory* global_ptr) {
    
    //Gets:Sets folder location for Repo location
    //Gets:Sets folder location for media import 
    printf("Select a location to save the Repository.\n");
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr)) {
        BrowseForFolder(global_ptr, 0);
        _tprintf(_T("Please select the media to import\n"));
        BrowseForFolder(global_ptr, 1);
        CoUninitialize();
    }

    //ERROR HANDLING
    Create_Folders(global_ptr);
    printf("\n");

    TCHAR copy[MAX_PATH];

    _tcscpy_s(copy, MAX_PATH, global_ptr->path_to_media_for_import);
    MoveMediaToMaster(copy);

}