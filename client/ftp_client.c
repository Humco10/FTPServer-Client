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

int main(int argc, char *argv[])
{
    //Create socket
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //Makes sure socket did not fail in creation.
    if (sock < 0)
    {
        printf("Failed to create the socket\n");
        exit(1);
    }

    //Create remote host address structure
    struct sockaddr_in sin;
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;                 //internet address type
    sin.sin_port = htons((uint16_t)argv[2]);  //set to port number
    sin.sin_addr.s_addr = inet_addr(argv[1]); //set ip address

    //Try to create connection with host given the server ip address
    int returncode = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
    if (returncode < 0)
    {
        printf("Failed to connect with the host\n");
        exit(2);
    }

    printf("Connection successfully established with the ftp server\n");

    //Waiting for user input
    char input[10];
    char location[10];
    while (1)
    {
        printf("ftp> ");
        scanf("%s", input);
        printf("CLIENT INPUT: %s\n", input);

        if (strcmp(input, "bye") == 0)
        {
            if (send(sock, input, sizeof(input), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            break;
        }

        scanf("%s", location);

        //A testing input to test server response.
        if (strcmp(input, "test") == 0)
        {
            if (send(sock, input, sizeof(input), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            //Waits to recieve message back from the server and displays it.
            if (recv(sock, input, sizeof(input), 1) > 0)
            {
                printf("test recieved: %s", input);
            }
            else
            {
                printf("Error recieving files");
            }
        }

        if (strcmp(input, "ls") == 0)
        {
            if (strcmp(location, "server") == 0)
            {
                //Send ls input to the server, display errors along the way if steps fail.
                if (send(sock, input, sizeof(input), 0) == -1)
                {
                    printf("Error in sending input to server");
                }
                else
                {
                    //printf("ls command sent to server\n");

                    //Waits to recieve message back from the server and displays it.
                    if (recv(sock, input, sizeof(input), 1) > 0)
                    {
                        printf("ls recieved: %s\n", input);
                    }
                    else
                    {
                        printf("Error recieving files\n");
                    }
                }
                printf("If you wish to download a file enter \"d\" followed by the file number\n");
            }
            else if (strcmp(location, "client") == 0)
            {
                //List files in a directory by reading them into this program
                //by creating a directory object, and then print them out in
                //order.
                struct dirent *dp;
                DIR *directory = opendir(".");
                int listNum = 1;

                //While there are still files in the directory
                while ((dp = readdir(directory)))
                {
                    //Determine is particular character is in string. This being the period
                    char *ptr = strchr(dp->d_name, '.');
                    int indexOfPeriod = -1;

                    //Get index of the period in the string
                    if (ptr)
                    {
                        indexOfPeriod = ptr - (dp->d_name);
                    }

                    //Don't include hidden files that begin with a period.
                    if (indexOfPeriod != 0)
                    {
                        printf("%d.  %s\n", listNum, dp->d_name);
                        listNum++;
                    }
                }

                closedir(directory);

                printf("If you wish to upload a file enter \"u\" followed by the file number");
            }
        }
        else if (strcmp(input, "d") == 0)
        {
            //download file to client
            char fileName[250];
            unsigned int numBytes = 0;

            printf("ftp> File \"%s\" downloaded successfully. %d bytes received.", fileName, numBytes);
        }
        else if (strcmp(input, "u") == 0)
        {
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