// UDP Client for an UDP Echo Server
#include <iostream.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

void main(void)
{
   struct sockaddr_in servAddr;
   unsigned int addrlen;
   int sock;
   struct hostent *hostPtr;
   char buffer[1000];
   int readBytes;
   int retcode;
   sock = socket(PF_INET, SOCK_DGRAM, 0);
   if (sock < 0)
   {
      cout << "Failed to create a socket" << endl;
      exit(1);
   }
   hostPtr = gethostbyname("penguin.tamucc.edu");
   if (hostPtr ==  NULL)
   {
      cout << "Failed to find host info" << endl;
      exit(1);
   }
   bzero((char *) &servAddr, sizeof(servAddr));
   servAddr.sin_family = AF_INET;
   bcopy(hostPtr->h_addr, (char *) &servAddr.sin_addr, hostPtr->h_length);
   servAddr.sin_port = htons(8888);
   addrlen = sizeof(servAddr);

   // Without bind() and connect(), we need to use
   // sendto() and recvfrom() for datagram exchanges
   // we cannot use write() and read() since they 
   // do not have any address parameter
char message[] = "Hello World";
   if (sendto(sock, message, sizeof(message)-1, 0, 
             (struct sockaddr*) &servAddr,addrlen) < 0)
   {
        cout << "Failed to send message" << endl; 
        exit(1);
   }
   cout << "Sent: " << message << endl;
   readBytes = recvfrom(sock,buffer,999,0,
                 (struct sockaddr*) hostPtr, &addrlen);
   if (readBytes < 0)
   {
        cout << "Failed to receive message back" << endl; 
        exit(1);
   }

   buffer[readBytes] = '\0';
   cout << "Received: " << buffer << endl;
}   
