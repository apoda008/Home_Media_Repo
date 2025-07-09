#include "folder_management.h"
#include "api_functions.h"
#include "networking.h"
#include "program_functions.h"


int main() {

    printf("Starting up Media Repository\n\n");
    
    printf("Establishing Pipe....\n");
    if (!pipe_to_server()) {
        printf("Failed to connect...");
    }
    
    
    return 0;
}