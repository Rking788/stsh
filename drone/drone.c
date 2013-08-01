#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define STSH_PORT   5777

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     // Create the socket file descriptor
     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
        fprintf(stderr, "ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
   
     portno = STSH_PORT;
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     // Bind the socket
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              fprintf(stderr,"ERROR on binding");
   
     // Start listening on the port
     listen(sockfd,5);

     clilen = sizeof(cli_addr);

     // Accept connections on the port/socket
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     
     if (newsockfd < 0) 
          fprintf(stderr, "ERROR on accept");
     
     bzero(buffer,256);
     
     // Read the message from the socket
     n = read(newsockfd,buffer,255);
     
     if (n < 0)
        fprintf(stderr, "ERROR reading from socket");
     
    printf("Here is the message: %s\n",buffer);

    n = write(newsockfd,"I got your message",18);
     
    if (n < 0)
        fprintf(stderr, "ERROR writing to socket");
     
     return 0; 
}
