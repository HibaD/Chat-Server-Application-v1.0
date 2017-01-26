#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXBUFLEN 100
#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char buf[MAXBUFLEN];
    int rv;
    int numbytes;
    if (argc != 3) {
        fprintf(stderr, "usage: talker hostname message\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));

        return 1;
    }
    // loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }
	/*char msg[BUFSIZE];
	bzero(msg, BUFSIZE);
	printf("ftp ");
	fgets(msg, BUFSIZE, stdin);*/
    //char* msg;
   // printf("ftp ");
    //scanf("%s", &msg);
    //const void *buf2 = "ftp";
   // if (access(msg, F_OK) != -1) {
        //send ftp to server
        if ((numbytes = sendto(sockfd, "ftp", 3, 0,
                p->ai_addr, p->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
        }
   // } else {
        //exit(1);
    //}
while(1){
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
            (struct sockaddr *) &their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
}

    if (!strcmp(buf,"yes")) {
        
        printf("A file transfer can start\n");
        
    } else{
        exit(1);
    }
        freeaddrinfo(servinfo);
        //printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    
        close(sockfd);
        return 0;
    
}
