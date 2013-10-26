#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// StackOverflow stuff
#include <fcntl.h>
#include <unistd.h>

// OpenCV Stuff
#include <opencv2/opencv.hpp>

#define STSH_PORT   5777
#define USEC_PERIOD 2000

void testOpenCV(useconds_t usecPeriod);
void testSOAnswer();
void testPipeToTerminal();
void setupListeningSocket();

int main(int argc, char *argv[])
{
    // OpenCV to open the iSight camera hopefully
    testOpenCV(USEC_PERIOD);

    // Code from a stackoverflow answer
    //testSOAnswer();

    // Test pipe stuff
    //testPipeToTerminal();

    // Setup a socket to start listening for connection and read from it
    //setupListeningSocket();
}

void testOpenCV(useconds_t usecPeriod)
{
    cv::VideoCapture stream;
    
    stream.open(0);

    if(!stream.isOpened()) {
        fprintf(stderr, "Failed to open VideoCapture stream\n");
        return;
    }

    usleep(usecPeriod);

    stream.release();

    //cv::Mat cameraFrame;
    //stream.read(cameraFrame);
    //cv::imshow("cam", cameraFrame);

    //cv::waitKey();
    //if (cv::waitKey(30) >= 0)
    //    break;
    //}

    //sleep(5);
}

void testSOAnswer()
{
    pid_t i;
    char buf[512];
    int fds, fdm, status;

    printf("Just inside\n");

    fdm = posix_openpt(O_RDWR);
    grantpt(fdm);
    unlockpt(fdm);

    printf("After pt stuff\n");

    close(0);
    close(1);
    close(2);

    printf("Pre-fork\n");

    i = fork();

    printf("Past fork\n");
    if ( i == 0 ) { // father
        dup(fdm);
        dup(fdm);
        dup(fdm);
        printf("Where do I pop up?\n");
        sleep(2);
        printf("Where do I pop up - 2?\n");
        waitpid(i, &status, 0);
    } 
    else {  // child
        fds = open(ptsname(fdm), O_RDWR);
        dup(fds);
        dup(fds);
        dup(fds);
        strcpy(buf, ptsname(fdm));
        //sprintf(buf, "xterm -S%c/2", basename(buf));
        //sprintf(buf, "/Applications/Utilities/Terminal.app/Contents/MacOS/Terminal -S%c/2", basename(buf));
        system(buf);
        exit(0);
    }
}

void testPipeToTerminal()
{
    int fds[2] = {0};

    // Create the pipe that will be used to write to the terminal
    int success = pipe(fds);
    if (success == 0) { 
        printf("Successfully created pipe with fds: %d, %d\n", fds[0], fds[1]);
    }
    else {
        fprintf(stderr, "Failed to create pipe, returning\n");
        return ;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child
        //close(stdin);

        int returned = dup2(0, fds[0]);
        printf("Returned %d\n", returned);
        execlp("/Applications/Utilities/Terminal.app/Contents/MacOS/Terminal", "/Applications/Utilities/Terminal.app/Contents/MacOS/Terminal", NULL);
    }
    else {
        // Parent
        dup2(1, fds[1]);

        while (1){
            //printf("Did it work?\n");
            write(fds[1], "This is a message\n", strlen("This is a message\n"));
        } 
    }
}

void setupListeningSocket()
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
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

     printf("Starting to listen on port: %d\n", htons(portno));
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
     
     return ; 
}
