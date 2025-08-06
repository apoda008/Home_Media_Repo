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
	//MediaData** hash_table = Hash_Initialization(500, dir_struct_ptr);

    //will initialize genre mapping here which will return an object with an array for each 
    //genre structured like MediaData* array_action[ hash_table[some var], hash_table[some var], 
    // hash_table[some var], ...]
    //To be implemented later 
    //primary goal is to get a video to stream, features after 
	
	//Api_Connection(hash_table, master_pathing.size_of_hash);

    //builds folders at requested location
    //moves Media from where it is to the constructed folders
    //TMDB the parsed media 
    //writes media to bin 
    //bin_media is instanced and put into a hash table
    //DB sets up Genre mapping
    //DB waits for commands 
    //..

    //TEST REGION

	DatabaseStructure* test = Construct_Database_Structure(10, 10);

    /*
	printf("Movie Set Size: %zu\n", test->movie_set_size);
	printf("Series Set Size: %zu\n", test->series_set_size);
	printf("Movie elements: %d\n", test->movies->num_elements_MV);
	printf("Series elements: %d\n", test->series->num_elements_Se);
    printf("\n");

    Insert_Movie(test, "fired up", "this is a description", _T("C://main//thing"), 4050);
    printf("Movie elements: %d\n", test->movies->num_elements_MV);
    Insert_Movie(test, "Not fired up", "this is a description2", _T("C://main//thing2"), 4050);
    printf("Movie elements: %d\n", test->movies->num_elements_MV);
    Insert_Movie(test, "fired up 2", "this is a description3", _T("C://main//thing3"), 4050);
    printf("Movie elements: %d\n", test->movies->num_elements_MV);
	printf("\n");

	printf("Movie ID: %d\n", test->movies->id[0]);
	printf("Movie Title: %s\n", test->movies->title[0]);
	printf("Movie Description: %s\n", test->movies->description[0]);
	printf("Movie Dir Position: %s\n", test->movies->dir_position[0]);
	printf("Movie Video Size: %d\n", test->movies->video_size[0]);

	printf("Movie ID: %d\n", test->movies->id[1]);
	printf("Movie Title: %s\n", test->movies->title[1]);
	printf("Movie Description: %s\n", test->movies->description[1]);
	printf("Movie Dir Position: %s\n", test->movies->dir_position[1]);
	printf("Movie Video Size: %d\n", test->movies->video_size[1]);

	printf("Movie ID: %d\n", test->movies->id[2]);
	printf("Movie Title: %s\n", test->movies->title[2]);
	printf("Movie Description: %s\n", test->movies->description[2]);
	printf("Movie Dir Position: %s\n", test->movies->dir_position[2]);
	printf("Movie Video Size: %d\n", test->movies->video_size[2]);
    */

    Fill_Table_Movies(test, dir_struct_ptr);
    Print_Movie_Table(test->movies);
    //END TEST REGION



    return 0;
}