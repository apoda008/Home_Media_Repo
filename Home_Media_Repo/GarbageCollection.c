#include <winsock2.h>
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
}
