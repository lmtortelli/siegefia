#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFLEN 2048
#define SERVICE_PORT	5005
#define MSGS 5	
#define IP "127.0.0.1"

int main(void)
{	struct sockaddr_in remaddr;
	int fd, i, slen=sizeof(remaddr);
	char buf[BUFLEN];	
	int recvlen;	

   if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
		printf("socket created\n");
		
	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(SERVICE_PORT);
	if (inet_aton(IP, &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

   sprintf(buf, "Hello");
   printf("Envia mensagem: %s\n",buf);
	if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, slen)==-1) 
	{	perror("sendto");
		exit(1);
	}
	close(fd);
	return 0;
}
