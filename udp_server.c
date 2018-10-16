//*******************************************
//
// Humberto Colin
// Computer Networks
// Assignment #3 : Server
// October 21, 2018
// Instructor: Dr. Ajay K. Katangur
//
//*******************************************


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>

void main() 
{
   int retcode;
   int sock;
   struct sockaddr_in servAddr;
   struct sockaddr_in clientAddr; 
   char buffer[500];
   unsigned int length;

   sock = socket(PF_INET, SOCK_DGRAM, 0);
   if (sock < 0) 
   { 
      cout << "Failed to create a socket" << endl;
      exit(1);
   }
// To reuse the same port again and again
   int i = 1;
   retcode = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
              (char *) &i, sizeof (i));
   if (retcode < 0) 
   { 
      cout << "Failed to set socket options for reuse" << endl;
      exit(1);
   } 
                   
   length = sizeof(servAddr);
   bzero((char *) &servAddr, length);
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servAddr.sin_port = htons(12000);      
   
   retcode = bind( sock, (struct sockaddr*)&servAddr, length);
   if (retcode < 0) 
   { 
      cout << "Failed to bind" << endl;
      exit(1);
   }
   int bytereceived;
   // receive and send datagrams in a loop
   while (1) 
   {
      bytereceived = recvfrom(sock, buffer, sizeof(buffer) - 1, 
                0, (struct sockaddr *) &clientAddr, &length);
      if (bytereceived < 1) 
      { 
         cout << "Failed to receive any message" << endl; 
         exit(1);
      }
      buffer[bytereceived] = '\0';
      // convert to upper case and send message back
      for(int count = 0; buffer[count] != '\0'; ++count)
         if(buffer[count] >= 'a' && buffer[count] <= 'z')
           buffer[count] = buffer[count] -('a' - 'A');
      sendto(sock, buffer, strlen(buffer), 0, 
               (struct sockaddr *) &clientAddr, sizeof(clientAddr));
    }
 }

