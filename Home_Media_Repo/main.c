#include "folder_management.h"
#include "api_functions.h"
#include "networking.h"
#include "program_functions.h"


int main() {
    printf("Starting up Media Repository\n\n");
    
    static struct Master_Directory master_pathing;
    static struct Master_Directory* dir_struct_ptr = &master_pathing;
    master_pathing.tmdb_limiter = 0;

    //printf("Select a location to save the Repository.\n");
    //HRESULT hr = CoInitialize(NULL);
    //if (SUCCEEDED(hr)) {
    //    BrowseForFolder(dir_struct_ptr, 0);
    //    _tprintf(_T("Please select the media to import"));
    //    BrowseForFolder(dir_struct_ptr, 1);
    //    CoUninitialize();
    //}

    FolderExecution(dir_struct_ptr);


    printf("\n\n\n");
    _tprintf(_T("Global dir MASTER: %s\n"), master_pathing.master_folder);
    _tprintf(_T("Global dir IMPORT: %s\n"), master_pathing.path_to_media_for_import);
    printf("Global tmdb counter: %d\n", master_pathing.tmdb_limiter);
    printf("Total files: %d\n", master_pathing.num_of_files);
    
    //REMEMBER TO PROPERLY SET THE SIZE
	MediaData** hash_table = Hash_Initialization(500, dir_struct_ptr);

    //will initialize genre mapping here which will return an object with an array for each 
    //genre structured like MediaData* array_action[ hash_table[some var], hash_table[some var], 
    // hash_table[some var], ...]
    //To be implemented later 
    //primary goal is to get a video to stream, features after 
	
	Api_Connection(hash_table, master_pathing.size_of_hash);

    //builds folders at requested location
    //moves Media from where it is to the constructed folders
    //TMDB the parsed media 
    //writes media to bin 
    //bin_media is instanced and put into a hash table
    //DB sets up Genre mapping
    //DB waits for commands 
    //..


    return 0;
}