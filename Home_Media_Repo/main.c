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
    static struct Master_Directory* global_ptr = &master_pathing;
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

    
    //hardcoded for now for both time and I need to ensure the folder selection works properly
    _tcsnccpy_s(global_ptr->master_folder, MAX_PATH, _T("C:\\Users\\dan_a\\Desktop"), MAX_PATH);
    _tcsnccpy_s(global_ptr->path_to_media_for_import, MAX_PATH, _T("C:\\Users\\dan_a\\Desktop\\testdirfolder"), MAX_PATH);
    
    //Creates folders if not already there 
    Create_Folders(global_ptr);
    
    //Creates the movies into mp4 regardless of their type for streaming
    Copy_To_Mp4(global_ptr->path_to_media_for_import, global_ptr);



    //DEPRECATED
    //FolderExecution(dir_struct_ptr);

    printf("\n\n\n");
    _tprintf(_T("Global dir MASTER: %s\n"), master_pathing.master_folder);
    _tprintf(_T("Global dir IMPORT: %s\n"), master_pathing.path_to_media_for_import);
    printf("Global tmdb counter: %d\n", master_pathing.tmdb_limiter);
    printf("Total files: %d\n", master_pathing.num_of_files);

 

    //para needs to be num of files 
	DatabaseStructure* test = Construct_Database_Structure(master_pathing.num_of_files, 10);
    
    Media_Files_Into_Table(global_ptr, test);

    //SAVE TABLE


	//fill constructed DB structure with movies from bin files
    //Fill_Table_Movies(test, global_ptr);
    
    //print constructed movie table
    Print_Movie_Table(test->movies);
    
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

