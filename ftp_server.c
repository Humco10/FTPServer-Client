//*******************************************
//
// Kyle Finter
// Computer Networks
// Assignment #3 : Server
// October 21, 2018
// Instructor: Dr. Ajay K. Katangur
//
//*******************************************
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main( int argc, char *argv[] )  {
    struct sockaddr_in server_sin;
    struct sockaddr_in client_sin;
    char buffer[500];
    int sockListen;
    int sockAccept;
    unsigned int addressLen;
    int length;
    const int Q_LEN = 5;

    //setup address structure
    bzero((char *) &server_sin, sizeof(server_sin));
    server_sin.sin_family = AF_INET;
    server_sin.sin_addr.s_addr = INADDR_ANY;
    server_sin.sin_port = htons((uint16_t) argv[1]);

    //setup listening socket
    sockListen = socket(PF_INET, SOCK_STREAM, 0);
    if (sockListen < 0) {
        printf("Failed to create listening socket\n");
        exit(1);
    }

    if (bind(sockListen, (struct sockaddr *) &server_sin, sizeof(server_sin)) < 0) {
        printf("Failed to bind listening socket to address\n");
        exit(2);
    }
    
    if (listen(sockListen, Q_LEN) < 0) {
        printf("Failed to listen\n");
        exit(3);
    }

    addressLen = sizeof(client_sin);

    //wait for connection request
    while(1){
        sockAccept = accept(sockListen, (struct sockaddr *) &client_sin, &addressLen);
        if (sockAccept < 0) {
            printf("Failed to accept connection\n");
            exit(4);
        }

        while(sockAccept > 0) {
            length = read(sockAccept, buffer, sizeof(buffer));
            if(length > 0) {
                int count;
                for(count = 0; count < length; ++ count) {
                    printf("%c\n", buffer[count]);
                }
                write(sockAccept, buffer, length);
                if(buffer == "bye") {
                    break;
                }
            }
            else
                break;
        }
        close(sockAccept);
    }
    return 0;
}