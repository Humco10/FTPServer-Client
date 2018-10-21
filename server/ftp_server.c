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
#include <dirent.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in server_sin;
    struct sockaddr_in client_sin;
    char buffer[500];
    int sockListen;
    int sockAccept;
    int num;
    unsigned int addressLen;
    int length;
    const int Q_LEN = 5;

    //Sets up the address structure of the server address
    bzero((char *)&server_sin, sizeof(server_sin));
    server_sin.sin_family = AF_INET;
    server_sin.sin_addr.s_addr = INADDR_ANY;
    server_sin.sin_port = htons((uint16_t)argv[1]);

    //Sets up a socket which listens for clients
    sockListen = socket(PF_INET, SOCK_STREAM, 0);

    //If socket was unable to be set up
    if (sockListen < 0)
    {
        printf("Failed to create listening socket\n");
        exit(1);
    }

    //If socket could not be effectively bound to the server address.
    if (bind(sockListen, (struct sockaddr *)&server_sin, sizeof(server_sin)) < 0)
    {
        printf("Failed to bind listening socket to address\n");
        exit(2);
    }

    //If socket is unable to start listening.
    if (listen(sockListen, Q_LEN) < 0)
    {
        printf("Failed to listen\n");
        exit(3);
    }

    addressLen = sizeof(client_sin);

    //Await a connection request, and then
    while (1)
    {
        sockAccept = accept(sockListen, (struct sockaddr *)&client_sin, &addressLen);
        if (sockAccept < 0)
        {
            printf("Failed to accept connection\n");
            exit(4);
        }

        while (sockAccept > 0)
        {
            if ((recv(sockAccept, buffer, sizeof(buffer), 0)) == -1)
            {
                perror("recv");
                exit(1);
            }

            if (strlen(buffer) > 0)
            {
                printf("SERVER RECIEVED: %s\n", buffer);

                if (strcmp(buffer, "ls") == 0)
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

                    if (send(sockAccept, buffer, sizeof(buffer), 1) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }

                    break;
                }

                if (strcmp(buffer, "test") == 0)
                {
                    if (send(sockAccept, buffer, sizeof(buffer), 1) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }
                }
                //int count;
                //for (count = 0; count < length; ++count)
                //{
                //printf("%c\n", buffer[count]);
                //}
                //write(sockAccept, buffer, length);
                if (strcmp(buffer, "bye") == 0)
                {
                    close(sockAccept);
                    exit(0);
                }
            }
            else
                break;
        }
        close(sockAccept);
    }
    return 0;
}