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
            printf("%s\n", buffer);

            if (strlen(buffer) > 0)
            {
                if (strcmp(buffer, "ls") == 0)
                {
                    //List files in a directory by reading them into this program
                    //by creating a directory object, and then print them out in
                    //order.
                    struct dirent *dp;
                    DIR *directory = opendir("./server/");
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
                    printf("\n");

                    //Go back to client
                    if (send(sockAccept, buffer, sizeof(buffer), 0) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }
                }
                else if (strcmp(buffer, "test") == 0)
                {
                    printf("This is buffer %s", buffer);
                    if (send(sockAccept, buffer, sizeof(buffer), 0) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }
                }
                else if (strcmp(buffer, "bye") == 0)
                {
                    close(sockAccept);
                    exit(0);
                }
                else if (strcmp(buffer, "d") == 0)
                {
                    /*
                    //Go back to client
                    if (send(sockAccept, buffer, sizeof(buffer), 0) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }*/

                    printf("Inside d\n");
                    //Recieve fileNumber from client
                    if ((recv(sockAccept, buffer, sizeof(buffer), 0)) == -1)
                    {
                        perror("recv");
                        exit(1);
                    }

                    char fileNumber[10];
                    strncpy(fileNumber, buffer, sizeof(fileNumber));
                    printf("%s", fileNumber);
                    
                    //Go back to client
                    if (send(sockAccept, buffer, sizeof(buffer), 0) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }
                    
                    
                }
                else if (strcmp(buffer, "u") == 0)
                {
                    //Recieve filename from client
                    if ((recv(sockAccept, buffer, sizeof(buffer), 0)) == -1)
                    {
                        perror("recv");
                        exit(1);
                    }

                    //Sets filename and resets buffer so it can be rewritten
                    char filename[500];
                    strncpy(filename, buffer, sizeof(filename));

                    memset(buffer, 0, sizeof(buffer));

                    strcat(buffer, "./server/");
                    strcat(buffer, filename);

                    FILE *fp;
                    fp = fopen(buffer, "w");

                    //Send control of the terminal back to the client
                    if (send(sockAccept, buffer, sizeof(buffer), 0) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }

                    memset(buffer, 0, sizeof(buffer));

                    //Recieve the file text from the client
                    while (1)
                    {
                        if ((recv(sockAccept, buffer, sizeof(buffer), 0)) == -1)
                        {
                            perror("recv");
                            exit(1);
                        }

                        //When buffer says end, that means to stop writing the file.
                        if (strcmp(buffer, "end") == 0)
                        {
                            break;
                        }

                        fprintf(fp, "%s", buffer);

                        memset(buffer, 0, sizeof(buffer));

                        //Send control of the terminal back to the client
                        if (send(sockAccept, buffer, sizeof(buffer), 0) == -1)
                        {
                            printf("Error sending buffer back to client");
                        }
                    }

                    fclose(fp);

                    //Send control of the terminal back to the client
                    if (send(sockAccept, buffer, sizeof(buffer), 0) == -1)
                    {
                        printf("Error sending buffer back to client");
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
    return 0;
}