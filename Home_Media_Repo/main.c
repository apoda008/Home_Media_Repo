#include "folder_management.h"
#include "api_functions.h"
#include "networking.h"
#include "program_functions.h"


int main() {
    printf("Starting up Media Repository\n\n");
    
    static struct Master_Directory master_pathing;
    static struct Master_Directory* dir_struct_ptr = &master_pathing;
 

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
    





    return 0;
}