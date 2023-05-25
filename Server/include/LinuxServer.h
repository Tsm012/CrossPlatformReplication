#include "AbstractServer.h"

#ifdef __linux__ 
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Server : public AbstractServer
{
public:
    void Run() {
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[BUFFER_LENGTH];
        const char* hello = "Hello from server";

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET,
            SO_REUSEADDR | SO_REUSEPORT, &opt,
            sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr*)&address,
            sizeof(address))
            < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        while (true) {
            if ((new_socket
                = accept(server_fd, (struct sockaddr*)&address,
                    (socklen_t*)&addrlen))
                < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            valread = read(new_socket, buffer, 4);
            printf("%s\n", buffer);
            send(new_socket, buffer, strlen(hello), 0);

            string message = buffer;

            if (message.find("stop") != std::string::npos) {
                break;
            }

            // closing the connected socket
            close(new_socket);
        }

        // closing the listening socket
        shutdown(server_fd, SHUT_RDWR);
    }
    void ProcessStuff(string* msg)
    {
        while (true) {
            printf("%s\n", msg->c_str());
            usleep(3 * MICROSECONDS_IN_SECOND);
        }
    }
};
#endif


