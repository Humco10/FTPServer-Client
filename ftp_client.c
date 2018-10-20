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
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <dirent.h>

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
    while (1) {
        printf("ftp> ");
        scanf("%s", input);
        if (strcmp(input, "bye") == 0) {
            break;
        }
        scanf("%s", location);
        if (strcmp(input, "ls") == 0) {
            if (strcmp(location, "server") == 0) {
                //list files on server

                printf("If you wish to download a file enter \"d\" followed by the file number");
            }
            else if (strcmp(location, "client") == 0) {
                //list files on client
                //create file to put list of files on client side from system command
                int defaultout = dup(1);
                printf("%d\n", defaultout);
                int commandOutput;
                commandOutput = open("ls.txt",O_RDWR | O_TRUNC | O_CREAT);
                int success = dup2(commandOutput, 1);
                printf("%d\n", success);
                system("ls");

                //print files to screen
                /*char file[250];
                int i = 0;
                while((fgets(file, 249, commandOutput)) != EOF) {
                    printf("%s", file);
                }*/
                dup2(defaultout, 1);
                close(commandOutput);
                close(defaultout);

                printf("If you wish to upload a file enter \"u\" followed by the file number");
            }
        }
        else if (strcmp(input, "d") == 0) {
            //download file to client
            char fileName[250];
            unsigned int numBytes = 0;

            printf("ftp> File \"%s\" downloaded successfully. %d bytes received.", fileName, numBytes);
        }
        else if (strcmp(input, "u") == 0) {
            //upload file to server
            char fileName[250];
            unsigned int numBytes = 0;

            printf("ftp> File \"%s\" uploaded successfully. %d bytes sent.", fileName, numBytes);
        }
        printf("\n");
    }

    //close socket
    printf("Closing the connection\n");
    close(sock);
    return 0;
}