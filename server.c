#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


int main() {

    int server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(server_file_descriptor, (struct sockaddr*) &server, sizeof(server));
    listen(server_file_descriptor, 128);

    while (1) {

        int client_file_descriptor = accept(server_file_descriptor, NULL, NULL);
        char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, world!";
        for (int bytes_sent = 0; bytes_sent < sizeof(response); bytes_sent += send(client_file_descriptor, response+bytes_sent, sizeof(response) - bytes_sent, 0));

        close(client_file_descriptor);

    }

    return 0;

}
