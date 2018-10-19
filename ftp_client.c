//*******************************************
//
// Kyle Finter
// Computer Networks
// Assignment #3 : Server
// October 21, 2018
// Instructor: Dr. Ajay K. Katangur
//
//*******************************************
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>

int main( int argc, char *argv[] )  {
    //Create socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Failed to create the socket\n");
        exit(1);
    }

    //Create remote host address structure
    struct sockaddr_in sin;
    bzero((char *) &sin, sizeof(sin));
    sin.sin_family = AF_INET; //internet address type
    sin.sin_port = htons((uint16_t) argv[2]); //set to port number
    sin.sin_addr.s_addr = inet_addr(argv[1]); //set ip address

    //create connection
    int returncode = connect(sock, (struct sockaddr *) &sin, sizeof(sin));
    if (returncode < 0) {
        printf("Failed to connect with the host\n");
        exit(2);
    }

    printf("Connection successfully established with the ftp server\n");

    //Waiting for user input
    char input[10];
    char location[10];
    while (strcmp(input, "bye") != 0) {
        printf("ftp> ");
        scanf("%s %s", &input, &location);
        if (strcmp(input, "ls") == 0) {
            if (strcmp(location, "server") == 0) {
                //list files on server

                printf("If you wish to download a file enter \"d\" followed by the file number");
            }
            else if (strcmp(location, "client") == 0) {
                //list files on client
                system("ls");
                printf("If you wish to upload a file enter \"u\" followed by the file number");
            }
        }
        else if (strcmp(input, "d") == 0) {
            //download file to client
            char fileName[250];
            unsigned int numBytes;

            printf("File \"%s\" downloaded successfully. %d bytes received.", fileName, numBytes);
        }
        else if (strcmp(input, "u") == 0) {
            //upload file to server
            char fileName[250];
            unsigned int numBytes;

            printf("File \"%s\" uploaded successfully. %d bytes sent.", fileName, numBytes);
        }
        printf("\n");
    }

    //close socket
    printf("Closing the connection\n");
    close(sock);
    return 0;
}