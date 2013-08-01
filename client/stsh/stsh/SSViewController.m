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
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = STSH_PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        NSLog(@"ERROR opening socket");
   
    server = gethostbyname("192.168.1.2");
    
    if (server == NULL) {
        NSLog(@"ERROR, no such host\n");
        return;
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        NSLog(@"ERROR connecting");
    
    char* msg = "Done It!";
    n = write(sockfd, msg, strlen(msg));
    
    if (n < 0)
        NSLog(@"ERROR writing to socket");
    
    char incoming[256];
    bzero(incoming,256);
    
    n = read(sockfd, incoming, 255);
    
    if (n < 0)
        NSLog(@"ERROR reading from socket");
    
    NSLog(@"%s\n", incoming);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
