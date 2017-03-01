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
#define MAXBUFLEN 1000
#define MAXPKLEN 4096


// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*) sa)->sin6_addr);
}



struct packet {
	unsigned int total_frag;
	unsigned int frag_no;
	unsigned int size;
	char* filename;
	char filedata[1000];
};

struct packet* extract (buffer){
	/*int i;
	int len = 0,*len[5]=0, count=0;
	 	
	struct packet temp;	
	for (i = 0; i < strlen (buffer); i++){
		if(buffer[i]==':'){
			len[count] = len;
			count++;
			len = 0;
		}
		len++;
	}
	i=0;
	j =0;
	char buf[len[0]];
	while(i<len[0]){
		buf[i] = buffer[j];
		i++;
		j++;

	}
	temp.total_frag = buf;
	j++;
	char buf1[len[1]];
	i =0; 
	while(i<len[1){
		buf1[i] = buffer[j];
		i++;
		j++;

	}
	temp.frag_no = buf1;
	j++;
	char buf2[len[2]];
	i =0; 
	while(i<len[1){
		buf2[i] = buffer[j];
		i++;
		j++;

	}
	temp.frag_no = buf1;
	j++;
	char buf1[len[1]];
	i =0; j= 0;
	while(i<len[1){
		buf1[i] = buffer[j];
		i++;
		j++;

	}
	temp.frag_no = buf1;*/

}

int main(int argc, char *argv[]) {

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[3];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }
        int optval = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof (int));
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
	char buffer[1000];

    if ((numbytes = recvfrom(sockfd, buffer, 1000, 0,
            (struct sockaddr *) &their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
	//struct packet* pt = extract (buffer);

    if ((numbytes = sendto(sockfd, "yes", 3, 0,
                (struct sockaddr *) &their_addr, addr_len)) == -1) {
            perror("talker: sendto");
            exit(1);
        }

    else {
        //reply with no to client
        if ((numbytes = sendto(sockfd, "no", 2, 0,
               (struct sockaddr *) &their_addr, addr_len)) == -1) {
            perror("talker: sendto");
            exit(1);

        }
    }

    freeaddrinfo(servinfo);
    close(sockfd);
    return 0;

}
