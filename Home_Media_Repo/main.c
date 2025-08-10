#include "folder_management.h"
#include "api_functions.h"
#include "networking.h"
#include "program_functions.h"


int main() {
    printf("Starting up Media Repository\n\n");
    
    static struct Master_Directory master_pathing;
    static struct Master_Directory* dir_struct_ptr = &master_pathing;
    master_pathing.tmdb_limiter = 0;
	master_pathing.num_of_files = 0;
	master_pathing.size_of_hash = 0; 

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

    //builds folders at requested location
    //moves Media from where it is to the constructed folders
    //TMDB the parsed media 
    //writes media to bin 
    //bin_media is instanced and put into a hash table
    //DB sets up Genre mapping
    //DB waits for commands 
    //..

    //para needs to be num of files 
	DatabaseStructure* test = Construct_Database_Structure(10, 10);

    Fill_Table_Movies(test, dir_struct_ptr);
    printf("\nBefore sorting:\n");
    Print_Movie_Table(test->movies);
	
	Sort_Movie_Table(test);
	printf("After sorting:\n");
    Print_Movie_Table(test->movies);
    
    return 0;
}