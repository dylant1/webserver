#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>

FILE *open_file(const char *dirname, struct dirent *dir, const char *mode) {
    char pathname[1024];   /* should alwys be big enough */
    FILE *fp;

    sprintf( pathname, "%s/%s", dirname, dir->d_name );
    fp = fopen( pathname, mode );
    return fp;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s [PORT] [ROOT_DIR]", argv[0]);
        exit(1);
    }

    int PORT = atoi(argv[1]);
    char *ROOT_DIR = argv[2];
    FILE *fs;
    struct dirent *ent;
    printf("Root_Dir: %s\n", ROOT_DIR);
    unsigned int client_length;
    struct sockaddr_in server;
    struct sockaddr_in client;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    client_length = sizeof(client);
    int server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    DIR* dir = opendir(ROOT_DIR);
    if (ENOENT == errno) {
        perror("Directory does not exist");
        exit(1);
    } 
    unsigned long size;
    if (!dir) {
        perror("Failed to opendir()");
    }


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

        char buff[1000000];
        char response[1000000];
        char *current = response;
        int bytes, chunk=4096, size=sizeof(response)/sizeof(char);
        int n;
        char *file_path;


        while ((n = read(client_file_descriptor, buff, sizeof(buff) - 1)) > 0) {
            buff[n] = '\0';
            printf("\n%s\n\n", buff);
            if (n < sizeof(buff) - 1) {
                break;
            }
        }
        char *request_type = strtok(buff, " ");
        char *path;
        char *tmp = strchr(buff, ' ');
        path = strtok(tmp, " ");

        if (strcmp(request_type, "GET") != 0) {
            perror("Not a GET request");

            sprintf(response, "HTTP/1.1 501 Not Implemented\r\n", 0);
            send(client_file_descriptor, response, strlen(response), 0);
            exit(1);
        }
        //printf("Request_type: %s\n", request_type);
        //printf("Path: %s\n", path);

        char sent[1000000];


        file_path = ROOT_DIR;

        if (strcmp(path, "/") == 0) {
            //serve index.html


            while ((ent = readdir(dir)) != NULL) {
                printf("%s\n", ent->d_name);
                if (strcmp(ent->d_name, "index.html") == 0) {
                    printf("0\n");

                    strcat(file_path,ent->d_name);

                    printf("1\n");
                    fs = fopen(file_path, "r+");

                    if (fs == NULL) {

                        printf("Couldn't open %s: %s\n", file_path, strerror(errno));
                        printf("2\n");
                    }
                    if (fs) {

                        printf("3\n");
                        do {
                            bytes = fread(current, sizeof(char), chunk, fs);
                            current += bytes;
                        }
                        while (bytes == chunk);
                    }



                    fclose(fs);
                    *current = '\0';
                    //printf("response: %s", response);
                    sprintf(sent, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\nContent-Type: text\\html\r\n%s\r\n\r\n",bytes, response);
                }
            }

            
        }

        printf("Data sent: \n%s", sent);




        //sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\nHello from the server", strlen("Hello from the server"));

        if (send(client_file_descriptor, sent, sizeof(sent), 0) == -1) {
        perror("Failed to send response to client");
        exit(1);
        }

        close(client_file_descriptor);
    }

    closedir(dir);
    return 0;
}

