#include<stdio.h>       //printf
#include<string.h>      //strlen
#include<sys/socket.h>  //socket
#include<arpa/inet.h>   //inet_addr
#include <unistd.h>

#define SOCKET_ADDRESS 5555
#define SOCKET_INETADDRESS "127.0.0.1"

int main(int argc , char *argv[]) {
    int connection_socket;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

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
