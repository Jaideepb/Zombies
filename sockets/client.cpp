#include<string.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<netdb.h>
#include<errno.h>
#include<stdio.h>
#include<sstream>

#define PORT "34603"

using namespace std;

int main() {
	struct addrinfo hint,*res;
	int sockfd,status;
	char buff[256],outbuff[256];
	ostringstream oss;
	memset(&outbuff,'\0',256);
	memset(&hint,0,sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;

	if((status=getaddrinfo("127.0.0.1",PORT,&hint,&res))!=0){
		cout<<"Error: getaddrinfo\n";
		exit(-1);
	}

	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	
	cout<<"client: sockid:"<<sockfd<<endl;

	if(sockfd==-1) {
		cout<<"Error: socket \n";
		exit(1);
	}
	int count=0;
	if(connect(sockfd,res->ai_addr,res->ai_addrlen)!=-1) {
			
		while ((status = recv(sockfd,&buff,sizeof(buff),0) !=0 ) && count<10) {
			count++;
			cout<<buff;
			snprintf(outbuff,sizeof(outbuff),"%d\n",count);
			send(sockfd,outbuff,strlen(outbuff),0);
			memset(buff,0,sizeof(buff));
			oss.clear();
		}
	}
	else 
		cout<<"Error: connect \n";
	
	close(sockfd);
	
	return 0;
}

