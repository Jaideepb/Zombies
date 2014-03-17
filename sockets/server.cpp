#include<string.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<netdb.h>
#include<errno.h>
#include<stdio.h>

#define MYPORT "34567"
#define BACKLOG 10

using namespace std;

int main() {
	struct addrinfo hints,*res;
	struct sockaddr_storage inaddr;
	socklen_t addr_size;
	int sockfd,infd,status;

	memset(&hints,0,sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_flags	= AI_PASSIVE;

	if((status=getaddrinfo("127.0.0.1",MYPORT,&hints,&res)!=0)) {
		cout<<"getaddrinfo error\n";
		exit(1);
	}	

	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);	
	if(sockfd==-1) {
		cout<<"Error: socket \n";
		exit(1);
	}

	bind(sockfd,res->ai_addr,res->ai_addrlen);

	listen(sockfd,BACKLOG);

	infd = accept(sockfd,(struct sockaddr*)&inaddr,&addr_size);
	
	if(send(infd,"Hello\n",6,0) == -1)
		perror("send");	

	return 0;
}
