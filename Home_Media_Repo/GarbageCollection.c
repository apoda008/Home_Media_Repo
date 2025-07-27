
//THIS IS AN EXAMPLE TO NATIVELY MAKE REST
// Which when I hate myself enough, I will 
// implement into the C# app to make it a full REST API

/*#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define HOST      "api.example.com"
#define PORT      "80"
#define RESOURCE  "/todos/1"

int main(void) {
    WSADATA wsa;
    SOCKET sock;
    struct addrinfo hints = { 0 }, * res, * rp;
    char request[512], buffer[4096];
    int n;

    // 1) Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 2) DNS lookup
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(HOST, PORT, &hints, &res);

    // 3) Connect
    for (rp = res; rp; rp = rp->ai_next) {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock == INVALID_SOCKET) continue;
        if (connect(sock, rp->ai_addr, (int)rp->ai_addrlen) == 0) break;
        closesocket(sock);
    }
    freeaddrinfo(res);
    if (!rp) { printf("Failed to connect\n"); return 1; }

    // 4) Build & send GET
    sprintf(request,
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Accept: application/json\r\n"
        "Connection: close\r\n\r\n",
        RESOURCE, HOST);
    send(sock, request, strlen(request), 0);

    // 5) Receive & print
    while ((n = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    // 6) Clean up
    closesocket(sock);
    WSACleanup();
    return 0;
}*/


//while (1) {
//	FILE* video_file = _tfopen("C:\\Users\\dan_a\\Desktop\\TestLocationForRepo\\Media_Repository\\Media_Movies\\U571.mp4", _T("rb"));
//
//	char* buffer = (char*)malloc(4096);
//
//	if (buffer == NULL) {
//		printf("Memory allocation failed for buffer.\n");
//		closesocket(client_socket);
//		continue; //skip to next iteration
//	}
//    size_t bytes_read = 0;
//	while ((bytes_read = fread(buffer, 1, 4096, video_file)) > 0) {
//		int bytes_sent = send(client_socket, buffer, bytes_read, 0);
//		if (bytes_sent == SOCKET_ERROR) {
//			printf("Failed to send video data: %d\n", WSAGetLastError());
//			free(buffer);
//			fclose(video_file);
//			return 1; //Error code for send failure
//		}
//		printf("Sent %zu bytes of video data.\n", bytes_sent);
//	}
//}