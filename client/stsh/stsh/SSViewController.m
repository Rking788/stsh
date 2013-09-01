//
//  SSViewController.m
//  stsh
//
//  Created by Rob King on 7/31/13.
//  Copyright (c) 2013 Rob King. All rights reserved.
//

#import "SSViewController.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define STSH_PORT 5777

@interface SSViewController ()

@end

@implementation SSViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidAppear:(BOOL)animated
{
    [self doWork];
}

- (void) doWork
{
    [self findAddresses];
    
    NSLog(@"All done");
//    
//    char* msg = "Done It!";
//    n = write(sockfd, msg, strlen(msg));
//    
//    if (n < 0)
//        NSLog(@"ERROR writing to socket");
//    
//    char incoming[256];
//    bzero(incoming,256);
//    
//    n = read(sockfd, incoming, 255);
//    
//    if (n < 0)
//        NSLog(@"ERROR reading from socket");
//    
//    NSLog(@"%s\n", incoming);
}

- (void)findAddresses
{
    int lastDigit = 0;
    int serversFound = 0;
    
    while(lastDigit < 256){
        int sockfd;
        struct hostent *server;
        struct sockaddr_in serv_addr;
        
        char ip[14];
        memset(ip, 0, 14);
        sprintf(ip, "192.168.1.%d", lastDigit);
        
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (sockfd < 0)
            NSLog(@"ERROR opening socket");
        
        server = gethostbyname(ip);

        if (server == NULL) {
            NSLog(@"ERROR, no such host\n");
            continue;
        }
        
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(STSH_PORT);
        
        bcopy((char *)server->h_addr,
              (char *)&serv_addr.sin_addr.s_addr,
              server->h_length);
        
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
            //NSLog(@"ERROR connecting");
            close(sockfd);
        }
        else {
            serversFound++;
            close(sockfd);
            NSLog(@"Found a connection : %s", ip);
        }
        
        lastDigit++;
    }
    
    NSLog(@"Checked %d ips", lastDigit);
    NSLog(@"Found %d hosts", serversFound);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
