#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define ECHOMAX 255

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in ServAddr;
	struct sockaddr_in CliAddr;
	unsigned int cliAddrLen;
	char echoBuf[ECHOMAX];
	unsigned short ServPort;
	int recvMsgSize;
	
	if(argc != 2){
		fprintf(stderr, "Usage: %s<UDP SERVPORT>\n", argv[0]);
		exit(0);
	}
	
	ServPort = atoi(argv[1]);
	
	if((sock=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		printf("socket() failed\n");
		exit(0);
	}
	
	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(ServPort);
	
	if(bind(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0){
		printf("bind() failed\n");
		exit(0);
	}
	
	printf("UDP echo server started!!!\n");
	for(;;){
		cliAddrLen = sizeof(CliAddr);
		if((recvMsgSize=recvfrom(sock, echoBuf, ECHOMAX, 0, (struct sockaddr *)&CliAddr, &cliAddrLen)) < 0){
			printf("recvfrom() failed\n");
			exit(0);
		}
		
		printf("Handing client: %s\n", inet_ntoa(CliAddr.sin_addr));
		
		if(sendto(sock, echoBuf, recvMsgSize, 0, (struct sockaddr *)&CliAddr, sizeof(CliAddr)) != recvMsgSize){
			printf("sendto() sent a different number of bytes than expected\n");
			exit(0);
		}
	}
}