#include "folder_management.h"
#include "api_functions.h"
#include "networking.h"
#include "program_functions.h"
#include "Parse.h"
////Test Headers/////


/// /////////////////

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
	DatabaseStructure* test = Construct_Database_Structure(master_pathing.num_of_files, 10);
    
	//fill constrcuted DB structure with movies from bin files
    Fill_Table_Movies(test, dir_struct_ptr);
    
    //print constructed movie table
    Print_Movie_Table(test->movies);
    
	printf("NUM OF ELEMENTS: %d\n", test->movies->num_elements_MV);
	//Needs restructuring to include the new tables instead of MediaData
    //Api_Connection();

	//////////////////////////////////////////TEST ZONE//////////////////////////////////////////
	
    //in full implementation this will be initialized in the Api_Connection() function
    parse_node* root_test = initialize_parse_tree();
    
	Api_Connection(test, root_test);

	Free_Database_Structure(test);
	free(root_test);    
    return 0;
}

