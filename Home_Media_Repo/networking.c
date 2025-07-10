#include "networking.h"

bool pipe_to_server() 
{	
	const char* pipeNamed = "\\\\.\\pipe\\MediaRepoPipe";

	HANDLE pipe = CreateNamedPipeA(
		pipeNamed,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		1,					//max instances
		4096, 4096,			//Output and input buffer size
		0,					//default time out 
		NULL				//default security
	);

	if (pipe == INVALID_HANDLE_VALUE) 
	{
		printf("CreateNamedPipe failed: %lu\n", GetLastError());
	}

	printf("Waiting for client to connect....\n");

	BOOL connected = ConnectNamedPipe(pipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

	if (connected) {
		
		char buffer[128];
		DWORD bytesRead;
		ReadFile(pipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
		buffer[bytesRead] = '\0';
		printf("Received from client: %s\n", buffer);

		const char* response = "Hello from server!";
		DWORD bytesWritten;
		WriteFile(pipe, response, (DWORD)strlen(response), &bytesWritten, NULL);
	}

	CloseHandle(pipe);
	return true;
}