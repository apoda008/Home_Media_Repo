#include "folder_management.h"
#include "networking.h"

int CallSystemWithTCHAR(const TCHAR* cmd) {
#ifdef UNICODE
    // Convert TCHAR (wchar_t*) to char*
    char narrow_cmd[512];
    WideCharToMultiByte(CP_ACP, 0, cmd, -1, narrow_cmd, sizeof(narrow_cmd), NULL, NULL);
    return system(narrow_cmd);
#else
    return system(cmd); // TCHAR is char, so this is fine
#endif
}

int Convert_to_Mp4(TCHAR* filename_path, TCHAR* title, Master_Directory* global_ptr) {
	//need to remove the file extension from the title
	//This will be used to transcode the video into mp4 format
	int strlen_title = _tcslen(title);
	
	title[strlen_title - 4] = '\0'; //removes the last 4 characters (assumes .mov, .avi, etc.)

    TCHAR* cmd[1024];

    _snwprintf_s(
        cmd,                 
        1024,                
        _TRUNCATE,          
        L"ffmpeg -i \"%s\" -c:v libx264 -c:a aac -movflags +faststart \"%s\\%s.mp4\"",
        filename_path,
        global_ptr->path_to_media,
        title
    );

    _tprintf(_T("\nCOMMAND: %s\n"), cmd);
    
    int result = CallSystemWithTCHAR(cmd);

    if (result != 0)
    {
        printf("\nVideo transcoding failed\n");
        return 0;
    }
    else {
        printf("\nVideo transcoding successful\n");
        TCHAR* output_video = _T("output_video.mp4");
        return output_video; //return the path to the transcoded video
    }

    return 1;
}

void Copy_To_Mp4(TCHAR* path, Master_Directory* global_ptr) {
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
            if ((strcmp(findFileData.cFileName, ".") == 0) || (strcmp(findFileData.cFileName, "..") == 0)) {
                continue;
            }

            TCHAR next_dir[MAX_PATH];

            _tcscpy_s(next_dir, MAX_PATH, path);
            _tcscat_s(next_dir, MAX_PATH, _T("\\"));
            _tcscat_s(next_dir, MAX_PATH, findFileData.cFileName);
            //_tprintf(_T("NEXT_DIR: %s\n"), next_dir);

            //check if it's a directory or a file
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                //_tprintf(_T("[DIR]: %s\n"), findFileData.cFileName);

				Copy_To_Mp4(next_dir, global_ptr); //recursive call for subdirectories

            }
            else {
                //_tprintf(_T("[FILE]: %s\n"), findFileData.cFileName);
                //This creates a directory path with the target filename
                TCHAR dest[MAX_PATH];
                _tcscpy_s(dest, MAX_PATH, path);
                _tcscat_s(dest, MAX_PATH, _T("\\"));
                _tcscat_s(dest, MAX_PATH, findFileData.cFileName);

				//Converts the file to mp4 for stream and moves it to the media folder
				Convert_to_Mp4(dest, findFileData.cFileName, global_ptr);

            }

        } while (FindNextFile(hFind, &findFileData) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES) {
            printf("FindNextFile Failed (%d)\n", GetLastError());
        }

        FindClose(hFind);
    }

}

bool BrowseForFolder(Master_Directory* ptr, int create_or_move) {
    
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
            //return true;
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
            //return true;
        }
    } 
    //return false; //cancelled or failed
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
                //_tprintf(_T("[DIR]: %s\n"), findFileData.cFileName);
             
                MoveMediaToMaster(next_dir, global_ptr);
        
            }
            else {
                //_tprintf(_T("[FILE]: %s\n"), findFileData.cFileName);
                
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
        return NULL;
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
        return 1;
    }
    else {
        do {

            //Skip "." ".." file names
            if ((strcmp(findFileData.cFileName, ".") == 0) || (strcmp(findFileData.cFileName, "..") == 0)) {
                continue;
            }

            //check if it's a directory or a file
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                //DO NOTHING, ALL MEDIA SHOULD BE IN A SINGLE TARGET 
                //DIRECTORY
            }
            else {
                
                //THIS WILL BE FILES
                //TAKE FILE NAME AND PARSE IT FOR SENDING TO TMDB
                TCHAR* parsed_name = Parse_Helper(findFileData.cFileName);
				//_tprintf(_T("Parsed Name: %s\n"), parsed_name);
                information_Request(parsed_name, global_ptr, findFileData.cFileName);
				global_ptr->num_of_files++;
               
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
	
    //return;
    //Gets:Sets folder location for Repo location
    //Gets:Sets folder location for media import 
    //URGENT: NEED TO FIX IF CANCELLED
    printf("Select a location to save the Repository.\n");

    //Will learn more about this function to more safely execute 
    // this code
    // HRESULT hr = CoInitialize(NULL);
   // if (SUCCEEDED(hr)) {
   //     
   //     if(BrowseForFolder(global_ptr, 0)){
			//printf("Success\n");
   //         _tprintf(_T("Please select the media to import\n"));
   //     }
   //     else { 
   //         return; 
   //     }
   //     
   //     if (BrowseForFolder(global_ptr, 1)) {
   //         printf("Success\n");
   //         //return; }
   //         CoUninitialize();
   //     }
   // }

    //hardcoded for now for both time and I need to ensure the folder selection works properly
	_tcsnccpy_s(global_ptr->master_folder, MAX_PATH, _T("C:\\Users\\dan_a\\Desktop\\TestLocationForRepo"), MAX_PATH);
    _tcsnccpy_s(global_ptr->path_to_media_for_import, MAX_PATH, _T("C:\\Users\\dan_a\\Desktop\\testdirfolder"), MAX_PATH);

    //ERROR HANDLING
    Create_Folders(global_ptr);
    printf("\n");

    TCHAR copy[MAX_PATH];

    _tcscpy_s(copy, MAX_PATH, global_ptr->path_to_media_for_import);
	
    //Media will not be moved. It will be copied to the master folder
	//once implemented this will be removed entirely
    //MoveMediaToMaster(copy, global_ptr);
	printf("Moving media to master folder...\n");
	printf("WARNING: This will take some time.\n");

	Copy_To_Mp4(global_ptr->path_to_media_for_import, global_ptr);
    
    //some point needs to ask user to do this
    File_Search_Parse(global_ptr);
    
}
