#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: exe [PORT]");
        exit(1);
    }

    int PORT = atoi(argv[1]);
    unsigned int client_length;
    struct sockaddr_in server;
    struct sockaddr_in client;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    client_length = sizeof(client);

    int server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_file_descriptor == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    if (bind(server_file_descriptor, (struct sockaddr*) &server, sizeof(server)) == -1) {
        perror("Failed to bind socket");
        exit(1);
    }

    if (listen(server_file_descriptor, 128) == -1) {
        perror("Failed to listen on socket");
        exit(1);
    }

    while (1) {
        printf("Waiting for connection on port %d\n", PORT);
        fflush(stdout);

        int client_file_descriptor = accept(server_file_descriptor, (struct sockaddr*) &client, &client_length);
        if (client_file_descriptor == -1) {
            perror("Failed to accept client connection");
            exit(1);
        }

        char buff[1000];
        int n;

        while ((n = read(client_file_descriptor, buff, sizeof(buff) - 1)) > 0) {
            buff[n] = '\0';
            printf("\n%s\n\n", buff);
            if (n < sizeof(buff) - 1) {
                break;
            }
        }
        char response[1000];
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\nHello from the server", strlen("Hello from the server"));

        if (send(client_file_descriptor, response , strlen(response), 0) == -1) {
            perror("Failed to send response to client");
            exit(1);
        }

        close(client_file_descriptor);
    }

    return 0;
}

