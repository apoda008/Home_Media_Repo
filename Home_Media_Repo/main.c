#include "folder_management.h"
#include "api_functions.h"
#include "networking.h"
#include "program_functions.h"
#include "Parse.h"
////Test Headers/////


/// /////////////////

int main() {
    
    printf("Starting up Media Database...\n\n");
    
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

    //This will be DELETED or turned into logs at a later date
    printf("\n\n");
    _tprintf(_T("Global dir MASTER: %s\n"), master_pathing.master_folder);
    _tprintf(_T("Global dir IMPORT: %s\n"), master_pathing.path_to_media_for_import);
    printf("Global tmdb counter: %d\n", master_pathing.tmdb_limiter);
    printf("Total files: %d\n\n", master_pathing.num_of_files);
 
    //Initialize the Database
	DatabaseStructure* database = Construct_Database_Structure(master_pathing.num_of_files, global_ptr->num_of_files);
    
    //NEEDS TO CHECK IF A Table.Bin file already exists and load from there. 
    
    
    //Takes all the converted Media files pings the TMDB api for their info and stores in the table
    Media_Files_Into_Table(global_ptr, database);

    //SAVE TABLE
    int result = Save_Database(database, global_ptr->temp_folder_path);
    if (result != 0) {
        perror("Failed to save database \n");

    }
    //print constructed movie table
    //Print_Movie_Table(database->movies);
    printf("\n");
    Better_Print_Table(database->movies);
    printf("\n");
    //Create config/SaveState for restarts of the program


    //initialize the parse tree 
    parse_node* root_parse = initialize_parse_tree();
    
    //open the API connection and wait for requests
	Api_Connection(database, root_parse);


	Free_Database_Structure(database);
	free(root_parse);    
    return 0;
}

