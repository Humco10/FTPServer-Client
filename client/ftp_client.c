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
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
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
            if (recv(sock, input, sizeof(input), 0) >= 0)
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
                int listNum = 1;
                char buffer[250];

                //Send ls input to the server, display errors along the way if steps fail.
                if (send(sock, input, sizeof(input), 0) == -1)
                {
                    printf("Error in sending input to server");
                }

                while (1)
                {
                    //Recieves name of file from server
                    if (recv(sock, buffer, sizeof(buffer), 0) >= 0)
                    {
                    }

                    //When server sends back end, end listing the files on the server.
                    if (strcmp(buffer, "end") == 0)
                    {
                        break;
                    }

                    if (strcmp(buffer, "") != 0)
                    {
                        printf("%d.  %s\n", listNum, buffer);
                        listNum++;
                    }

                    //Send anything back to server to say that the file name was recieved.
                    if (send(sock, input, sizeof(input), 0) == -1)
                    {
                        printf("Error in sending input to server");
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
                DIR *directory = opendir("./client/");
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
            //Tell server what operation is occurring
            if (send(sock, input, sizeof(input), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            struct dirent *dp;
            DIR *directory = opendir("./server/");
            int fileIndex = 1;
            char buffer[500];

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
                    char tempStr[5];
                    snprintf(tempStr, sizeof(tempStr), "%d", fileIndex);

                    //If indicies match break so that dp->name is saved to the correct filename
                    if (strcmp(location, tempStr) == 0)
                    {
                        break;
                    }

                    fileIndex++;
                }
            }

            closedir(directory);

            //Send filename to server so it can create a file by the same name
            if (send(sock, dp->d_name, sizeof(dp->d_name), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            //Waits to recieve message back from the server before continuing
            if (recv(sock, buffer, sizeof(buffer), 0) >= 0)
            {
                //Keeps sending an error called "no error" back so I'm ignoring it for now
            }

            memset(buffer, 0, sizeof(buffer));

            strcat(buffer, "./server/");
            strcat(buffer, dp->d_name);

            //Read file chosen into the buffer (named input) to be sent to the server
            FILE *f;
            f = fopen(buffer, "r");
            int byteCount = 0;

            //Keep reading file till it is completely written.
            while (fgets(buffer, sizeof(buffer), f))
            {
                //Send file buffer to server to be written to the server files.
                if (send(sock, buffer, sizeof(buffer), 0) == -1)
                {
                    printf("Error in sending input to server");
                }

                //Waits to recieve message back from the server before continuing
                if (recv(sock, buffer, sizeof(buffer), 0) >= 0)
                {
                    //Keeps sending an error called "no error" back so I'm ignoring it for now
                }
            }

            //Tell server to stop writing to the file
            if (send(sock, "end", sizeof("end"), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            //Waits to recieve message back from the server before continuing
            if (recv(sock, buffer, sizeof(buffer), 0) >= 0)
            {
                //Keeps sending an error called "no error" back so I'm ignoring it for now
            }

            rewind(f);
            fseek(f, 0L, SEEK_END);
            byteCount = ftell(f);

            printf("ftp> File \"%s\" downloaded successfully. %d bytes sent.", dp->d_name, byteCount);

            fclose(f);
        }
        else if (strcmp(input, "u") == 0)
        {
            //Tell server what operation is occurring
            if (send(sock, input, sizeof(input), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            //List files in a directory by reading them into this program
            //by creating a directory object, and then print them out in
            //order.
            struct dirent *dp;
            DIR *directory = opendir("./client/");
            int fileIndex = 1;
            char buffer[500];

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
                    char tempStr[5];
                    snprintf(tempStr, sizeof(tempStr), "%d", fileIndex);

                    //If indicies match break so that dp->name is saved to the correct filename
                    if (strcmp(location, tempStr) == 0)
                    {
                        break;
                    }

                    fileIndex++;
                }
            }

            closedir(directory);

            //Send filename to server so it can create a file by the same name
            if (send(sock, dp->d_name, sizeof(dp->d_name), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            //Waits to recieve message back from the server before continuing
            if (recv(sock, buffer, sizeof(buffer), 0) >= 0)
            {
                //Keeps sending an error called "no error" back so I'm ignoring it for now
            }

            memset(buffer, 0, sizeof(buffer));

            strcat(buffer, "./client/");
            strcat(buffer, dp->d_name);

            //Read file chosen into the buffer (named input) to be sent to the server
            FILE *f;
            f = fopen(buffer, "r");
            int byteCount = 0;

            //Keep reading file till it is completely written.
            while (fgets(buffer, sizeof(buffer), f))
            {
                //Send file buffer to server to be written to the server files.
                if (send(sock, buffer, sizeof(buffer), 0) == -1)
                {
                    printf("Error in sending input to server");
                }

                //Waits to recieve message back from the server before continuing
                if (recv(sock, buffer, sizeof(buffer), 0) >= 0)
                {
                    //Keeps sending an error called "no error" back so I'm ignoring it for now
                }
            }

            //Tell server to stop writing to the file
            if (send(sock, "end", sizeof("end"), 0) == -1)
            {
                printf("Error in sending input to server");
            }

            //Waits to recieve message back from the server before continuing
            if (recv(sock, buffer, sizeof(buffer), 0) >= 0)
            {
                //Keeps sending an error called "no error" back so I'm ignoring it for now
            }

            rewind(f);
            fseek(f, 0L, SEEK_END);
            byteCount = ftell(f);

            printf("ftp> File \"%s\" uploaded successfully. %d bytes sent.", dp->d_name, byteCount);

            fclose(f);
        }
        printf("\n");
    }

    //close socket
    printf("Closing the connection\n");
    close(sock);
    return 0;
}