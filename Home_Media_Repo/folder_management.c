#include "folder_management.h"
#include "networking.h"


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

    //media_bin directory
    _tcscpy_s(global_ptr->movie_bin_path, MAX_PATH, global_ptr->path_to_media);
    _tcscat_s(global_ptr->movie_bin_path, MAX_PATH, _T("\\Movies_bin"));


    if (CreateDirectory(global_ptr->movie_bin_path, NULL)) {
        _tprintf(TEXT("\nFolder created successfully: %s\n"), global_ptr->movie_bin_path);

    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            _tprintf(TEXT("\nDatabase folder already exists: %s\n"), global_ptr->movie_bin_path);

        }
        else {
            _tprintf(TEXT("\nfailed: %s\n"), global_ptr->movie_bin_path);

        }
    }
    
    //Temp directory
    _tcscpy_s(global_ptr->temp_folder_path, MAX_PATH, global_ptr->master_folder);
    _tcscat_s(global_ptr->temp_folder_path, MAX_PATH, _T("\\Temp"));


    if (CreateDirectory(global_ptr->temp_folder_path, NULL)) {
        _tprintf(TEXT("\nFolder created successfully: %s\n"), global_ptr->temp_folder_path);

    }
    else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            _tprintf(TEXT("\nDatabase folder already exists: %s\n"), global_ptr->temp_folder_path);

        }
        else {
            _tprintf(TEXT("\nfailed: %s\n"), global_ptr->temp_folder_path);

        }
    }
}

void MoveMediaToMaster(TCHAR* path, Master_Directory* global_ptr) {
   
    TCHAR copy_path[MAX_PATH];

    //sets the * for a search through all files in the directory
    _tcscpy_s(copy_path, MAX_PATH, path);
    _tcscat_s(copy_path, MAX_PATH, _T("\\*"));

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(copy_path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Error handle value. Error: %lu \n"), GetLastError());
    }
    else {
        do {
            
            //Skip "." ".." file names
            if ( (strcmp(findFileData.cFileName, ".") == 0) || (strcmp(findFileData.cFileName, "..") == 0 ) ) {
                continue;
            }
            
            TCHAR next_dir[MAX_PATH];

            _tcscpy_s(next_dir, MAX_PATH, path);
            _tcscat_s(next_dir, MAX_PATH, _T("\\"));
            _tcscat_s(next_dir, MAX_PATH, findFileData.cFileName);
            //_tprintf(_T("NEXT_DIR: %s\n"), next_dir);
            
            //check if it's a directory or a file
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                _tprintf(_T("[DIR]: %s\n"), findFileData.cFileName);
             
                MoveMediaToMaster(next_dir, global_ptr);
              
            }
            else {
                _tprintf(_T("[FILE]: %s\n"), findFileData.cFileName);
                
                //This creates a directory path with the target filename
                //MovieFile does not auto concat the filename
                TCHAR dest[MAX_PATH];
                _tcscpy_s(dest, MAX_PATH, global_ptr->path_to_media);
                _tcscat_s(dest, MAX_PATH, _T("\\"));
                _tcscat_s(dest, MAX_PATH, findFileData.cFileName);
                               
                //This is when we would move them all to the primary file
                if (MoveFile(next_dir, dest)) {
                    _tprintf(_T("Moved: %s\n"), findFileData.cFileName);
                } 
                else {
                    _tprintf(_T("Failed to move: %s (Error: %lu)"), findFileData.cFileName, GetLastError());
                }
       
            }
            
        } while (FindNextFile(hFind, &findFileData) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES) {
            printf("FindNextFile Failed (%d)\n", GetLastError());
        }

        FindClose(hFind);
    }

   

}

TCHAR* Parse_Helper(TCHAR* title) {
    size_t nameLen = _tcslen(title);
    TCHAR newFileName[MAX_PATH] = { 0 };
    //NEEDS DYNAMIC ALLOCATION. WILL BE FREED WHEN TMDB IS CALLED

    for (int i = 0; i < (nameLen - 3); i++) {
        if (title[i] == '.' || title[i] == '_') {
            newFileName[i] = ' ';
            //When it detects junk it sets the string terminator and breaks loop
            //its trying to detect things like Movie Title.2014DVD.mkv
            if ((isdigit(title[i + 1])) && (isdigit(title[i + 2])) && (isdigit(title[i + 3]))) {
                newFileName[i] = '\0';
                break;
            }
        }
        
        //'()' are not in normal titles, cut when detected
        else if (title[i] == '(') {
            newFileName[i] = '\0';
            break;
        }
        //will need a condition in here for '_'

        else {
            //title is normal so it stores it in the new TCHAR
            newFileName[i] = title[i];
        }
    }

    //THIS will probably be moved into a tokenization 
    TCHAR buffertwo[MAX_PATH] = { 0 };
    size_t j = 0;

    for (int i = 0; i < nameLen; i++) {
        if ((newFileName[i] == ' ') && (newFileName[i + 1] == '\0')) {
            break;
        }

        if ((newFileName[i] == ' ') && (newFileName[i + 1] != '\0')) {
            buffertwo[j] = '%';
            buffertwo[j + 1] = '2';
            buffertwo[j + 2] = '0';
            j += 3;
        }
        else {
            buffertwo[j] = newFileName[i];
            j++;
        }

    }
    buffertwo[j] = '\0';
    TCHAR* return_char_ptr = (TCHAR*)malloc((j + 1) * sizeof(TCHAR));
    if (return_char_ptr) {
        _tcscpy_s(return_char_ptr, (j + 1), buffertwo);
    }
    else {
		printf("Memory allocation failed for return_char_ptr.\n");
    }

    
    return return_char_ptr;
}

int File_Search_Parse(Master_Directory* global_ptr) {
    TCHAR copy_path[MAX_PATH];
    _tcscpy_s(copy_path, MAX_PATH,global_ptr->path_to_media);
    _tcscat_s(copy_path, MAX_PATH, _T("\\*"));

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(copy_path, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Error handle value. Error: %lu \n"), GetLastError());
    }
    else {
        do {

            //Skip "." ".." file names
            if ((strcmp(findFileData.cFileName, ".") == 0) || (strcmp(findFileData.cFileName, "..") == 0)) {
                continue;
            }

            //check if it's a directory or a file
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                _tprintf(_T("[DIR_in_Search]: %s\n"), findFileData.cFileName);
                //DO NOTHING, ALL MEDIA SHOULD BE IN A SINGLE TARGET 
                //FOLDER
            }
            else {
                _tprintf(_T("About to parse file\n"));
                //THIS WILL BE FILES
                //TAKE FILE NAME AND PARSE IT FOR SENDING TO TMDB
                TCHAR* parsed_name = Parse_Helper(findFileData.cFileName);
                information_Request(parsed_name, global_ptr);
                _tprintf(_T("Parsed: %s\n"), parsed_name);
                
            }

        } while (FindNextFile(hFind, &findFileData) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES) {
            printf("FindNextFile Failed (%d)\n", GetLastError());
        }

        FindClose(hFind);
    }
    return 0;
}


void FolderExecution(Master_Directory* global_ptr) {
    
    //Gets:Sets folder location for Repo location
    //Gets:Sets folder location for media import 
    //URGENT: NEED TO FIX IF CANCELLED
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
    MoveMediaToMaster(copy, global_ptr);
    
    //some point needs to ask user to do this
    File_Search_Parse(global_ptr);
    
}
