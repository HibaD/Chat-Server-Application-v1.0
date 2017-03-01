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
#include <sys/time.h>
#include <math.h>

#define MAXBUFLEN 100
#define BUFSIZE 1024

double delay(struct timeval t1, struct timeval t2);

struct packet {
	unsigned int total_frag;
	unsigned int frag_no;
	unsigned int size;
	char* filename;
	char filedata[1000];
};

void data(char* fileData, char* buffer, int len){
	
	int i,j;
	for (i = len+1, j=0; j<strlen(fileData); j++, i++){
		buffer[i] = fileData[j];
	}
	buffer[i] = '\0';	
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; 
    socklen_t addr_len;
    char buf[MAXBUFLEN];
    int rv;
    int numbytes;
	struct timeval start, end;

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
    
    char buf3[BUFSIZE];
    printf("ftp ");
    scanf("%s", buf3);
   
    int i=0;
    while (buf3[i] != '\0'){
        i++;
    }
    char new[i];
    while(i){
        new[i] = buf3[i];
        i--;
    }
    new[0] = buf3[0];

    if (access(new, F_OK) != -1) {
        //send ftp to server
		
		gettimeofday (&start, NULL);
		FILE *fp;
		fp = fopen (new, "rb");
		fseek (fp, 0, SEEK_END);
		int fileSize = ftell(fp);
		rewind (fp);

		struct packet pt;
		pt.filename = new;
		
		//printf ("File Size is %d \n", fileSize);
		if (fileSize >= 1000){
			pt.total_frag = filesize/1000;
		}

		else{
			pt.total_frag = (filesize/1000)+1;
			/*pt.size = fileSize;
			fread(pt.filedata, pt.size+1, 1, fp);
			pt.frag_no = 1;
			pt.total_frag = 1;
			char* buffer = malloc (3*sizeof(int)+4*sizeof(char)+strlen(pt.filename)+strlen(pt.filedata));
			int len = 3*sizeof(char)+4*sizeof(char)+strlen(pt.filename);
			sprintf(buffer, "%d:%d:%d:%s:", pt.total_frag, pt.frag_no, pt.size, pt.filename);
			
			//printf ("%s\n", buffer);
			//printf ("%s\n", pt.filedata);
			char* d = pt.filedata;			

			data (d, buffer, len);
			printf("%s\n", buffer);
        	if (numbytes = sendto(sockfd, buffer, strlen(buffer), 0,
               	p->ai_addr, p->ai_addrlen) == -1) {
            		perror("talker: sendto");
            		exit(1);
        	}*/
		}
    } else {
        exit(1);
    }
	for (pt.frag_no = 1; pt.frag_no <= )
    if ((numbytes = recvfrom(sockfd, buf, strlen(buf), 0,
            (struct sockaddr *) &their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

	gettimeofday (&end, NULL);

	printf ("Round-trip delay = %lf ms.\n", delay(start, end));
    if (buf[0] == 'y' && buf[1] == 'e' && buf[2] == 's') {

        printf("A file transfer can start\n");

    } else {
        exit(1);
    }
    freeaddrinfo(servinfo);
    //printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);

    close(sockfd);
    return 0;

}

double delay(struct timeval t1, struct timeval t2){
	double d;
	
	d = (t2.tv_sec - t1.tv_sec)*1000.0;
	d += ((t2.tv_usec - t1.tv_usec +500.0)/1000.0);
	return (d);
}




