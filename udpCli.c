#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#define ECHOMAX 10

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in ServAddr;
	struct sockaddr_in fromAddr;
	unsigned short ServPort;
	unsigned int fromSize;
	char *servIP;
	char *echoStr;
	char echoBuf[ECHOMAX + 1];
	int echoStrlen, recvStrlen;

	servIP = argv[1];
	ServPort = atoi(argv[2]);

	if((echoStrlen = strlen(echoStr)) > ECHOMAX){
		printf("Echo word too long\n");
		exit(0);
	}

	if((sock=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		printf("socket() failed\n");
		exit(0);
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(servIP);
	ServAddr.sin_port = htons(ServPort);

	if(sendto(sock, echoStr, echoStrlen, 0, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) != echoStrlen){
		printf("Error: The number of bytes unmatched!!!\n");
		exit(0);
	}

	fromSize = sizeof(fromAddr);
	
	if((recvStrlen=recvfrom(sock, echoBuf, ECHOMAX, 0, (struct sockaddr *)&fromAddr, &fromSize)) != echoStrlen){
		printf("recvfrom() failed\n");
		exit(0);
	}
	
	if(ServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr){
		fprintf(stderr, "Error: Packet from unkown source\n");
		exit(1);
	}	
	
	while(1){
		fputs("Input any string(q: Quit) ", stdout);
		fgets(echoBuf, ECHOMAX, stdin);
				
		if(!strcmp(echoBuf, "q\n"))
			break;
			
		int count = 0;
		for(int i=0; i<strlen(echoBuf); i++)
			if(!isspace(echoBuf[i])) count++;
			
		printf("Received: %s", echoBuf);
		printf("String Length: %d\n", count);
	}
	
	close(sock);
	exit(0);
}