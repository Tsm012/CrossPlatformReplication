#include "AbstractServer.h"

#ifdef _WIN32
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

class Server : public AbstractServer
{
public:
    void Run() {
        WSADATA wsaData;
        int iResult;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo* result = NULL;
        struct addrinfo hints;

        int iSendResult;
        char recvbuf[BUFFER_LENGTH];
        int recvbuflen = BUFFER_LENGTH;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            exit(EXIT_FAILURE);
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        const string port = to_string(PORT);

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // Create a SOCKET for the server to listen for client connections.
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // No longer need server socket
        closesocket(ListenSocket);

        // Receive until the peer shuts down the connection
        do {

            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);
                printf("Message %.*s\n", iResult, recvbuf);

                string reply = convertToString(recvbuf, iResult);

                printf("New %s\n", reply.c_str());

                // Echo the buffer back to the sender
                iSendResult = send(ClientSocket, reply.c_str(), static_cast<int>(reply.length()), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    exit(EXIT_FAILURE);
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (iResult == 0)
                printf("Connection closing...\n");
            else {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                exit(EXIT_FAILURE);
            }

        } while (iResult > 0);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // cleanup
        closesocket(ClientSocket);
        WSACleanup();
    }
    void ProcessStuff(string* message)
    {
        while (true) {
            printf("%s\n", message->c_str());
            Sleep(3 * MICROSECONDS_IN_SECOND);
        }
    }
private:
    string convertToString(char* a, int size)
    {
        int i;
        string s = "";
        for (i = 0; i < size; i++) {
            s = s + a[i];
        }
        return s;
    }
};
#endif