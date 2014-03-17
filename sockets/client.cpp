#include<string.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<netdb.h>
#include<errno.h>
#include<stdio.h>

#define PORT "34567"

using namespace std;

int main() {
	struct addrinfo hint,*res;
	int sockfd,status;
	char buff[256];

	memset(&hint,0,sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;

	if((status=getaddrinfo("127.0.0.1",PORT,&hint,&res))!=0){
		cout<<"Error: getaddrinfo\n";
		exit(-1);
	}

	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	connect(sockfd,res->ai_addr,res->ai_addrlen);
	
	recv(sockfd,&buff,sizeof(buff),0);

	cout<<buff;
	return 0;
}

