#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 2048
#define SERVICE_PORT	5005
#define IP "127.0.0.1"

int main(int argc, char **argv)
{	struct sockaddr_in myaddr;	
	struct sockaddr_in remaddr;	
	socklen_t addrlen = sizeof(remaddr);		
	int recvlen;			
	int fd;		
	unsigned char buf[BUFSIZE];	

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{	perror("cannot create socket\n");
		return 0;
	}

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr =  inet_addr(IP);
	myaddr.sin_port = htons(SERVICE_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) 
	{	perror("bind failed");
		return 0;
	}

	   recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0) 
		{	buf[recvlen] = 0;
			printf("received message: \"%s\"\n", buf);
		}
		else
			printf("something went wrong!\n");		
	
   close(fd);	
	return 0;
}
