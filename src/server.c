#include<stdio.h>
#include<stdlib.h>
#include<string.h>      //strlen
#include<sys/socket.h>
#include<arpa/inet.h>   //inet_addr
#include<unistd.h>      //write
#include<pthread.h>     //for threading , link with lpthread

#include "worker.h"

#define SOCKET_ADDRESS 5555

int main(int argc, char *argv[]) {
    // Descriptors of the client and the server socket.
    int server_socket, client_socket, *tmp_socket, accept_structure, connection_port;
    // sockaddr_in structures for client and server.
    struct sockaddr_in server , client;

    // Connection port can be set at runtime.
    connection_port = (argc > 1) ? (int) argv[1] : SOCKET_ADDRESS;

    // Information message about the current instance.
    printf("Initiating server on port: %u with PID: %u\n", connection_port, (int) getpid());

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        printf("Could not create socket for the server.\n");
    }
    printf("Server socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SOCKET_ADDRESS);

    // Bind sockets and addresses.
    if(bind(server_socket, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
        printf("Socket bind failed. Error occured.\n");
        return 1;
    }
    printf("Bind done.\n");

    listen(server_socket, 3);
    printf("Waiting for incoming connections.\n");

    accept_structure = sizeof(client);
    while( (client_socket = accept(server_socket, (struct sockaddr *) &client, (socklen_t *) &accept_structure)) ) {
        printf("Connection accepted.\n");

        pthread_t worker_thread;
        tmp_socket = malloc(1);
        *tmp_socket = client_socket;

        if(pthread_create(&worker_thread, NULL, connection_handler, (void *) tmp_socket) < 0) {
            printf("Could not create worker thread.\n");
            return 1;
        }
        printf("Worker assigned to the connection.\n");
    }

    if(client_socket < 0) {
        printf("Accept failed.\n");
        return 1;
    }
    return 0;
}
