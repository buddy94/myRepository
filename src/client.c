#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SOCKET_ADDRESS 5555
#define SOCKET_INETADDRESS "127.0.0.1"

int main(int argc , char *argv[]) {
    int connection_socket, connection_port;
    struct sockaddr_in server;
    char message[1000], server_reply[2000], *connection_addr;

    // Connection address and port can be set an runtime.
    connection_addr = (argc > 2) ? argv[2] : SOCKET_INETADDRESS;
    connection_port = (argc > 1) ? (int) argv[1] : SOCKET_ADDRESS;

    // Information message about the current instance.
    printf(
        "Creating client instance for a server at: %s:%u with PID: %u\n",
        connection_addr, connection_port, (int) getpid()
    );

    // Create the socket.
    connection_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (connection_socket == -1) {
        printf("Could not create socket.\n");
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(SOCKET_INETADDRESS);
    server.sin_family = AF_INET;
    server.sin_port = htons(SOCKET_ADDRESS);

    // Connect to remote server.
    if (connect(connection_socket, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("Connection failed. Error occured.");
        return 1;
    }
    printf("Connected.\n");

    // Keep communicating with server
    while(1) {
        printf("Enter message: ");
        fgets(message, 1024, stdin);

        //Send some data
        if(send(connection_socket, message, strlen(message)+1, 0) < 0) {
            printf("Send failed.\n");
            return 1;
        }

        //Receive a reply from the server
        if(recv(connection_socket, server_reply, sizeof(server_reply)+1, 0) < 0) {
            printf("Receive failed.\n");
            break;
        }

        printf("Server reply: ");
        puts(server_reply);
        memset(server_reply, 0, sizeof(server_reply));
    }
    close(connection_socket);
    return 0;
}
