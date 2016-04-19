#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void *connection_handler(void *socket_descriptor) {
    int connection_socket = *(int *) socket_descriptor;
    int read_size;
    char client_message[1024];

    //Receive a message from client
    while((read_size = recv(connection_socket, client_message, sizeof(client_message)+1, 0)) > 0) {
        //Send the message back to client and empty the message buffer.
        write(connection_socket , client_message, strlen(client_message)+1);
        memset(client_message, 0, sizeof(client_message));
    }

    if(read_size == 0) {
        printf("Client disconnected.\n");
        fflush(stdout);
    } else if(read_size == -1) {
        printf("Receive failed.\n");
    }
    //Free the socket pointer
    free(socket_descriptor);
    return 0;
}
