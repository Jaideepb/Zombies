#include<string.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<netdb.h>
#include<errno.h>
#include<stdio.h>
#include<unistd.h>

#define MYPORT "34605"
#define BACKLOG 10

using namespace std;

int main() {
	struct addrinfo hints,*res;
	struct sockaddr_storage inaddr;
	socklen_t addr_size;
	int sockfd,infd,status;
	char buff[256];

	memset(&buff,'\0',256);
	memset(&hints,0,sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags	= AI_PASSIVE;

	if((status=getaddrinfo(NULL,MYPORT,&hints,&res)!=0)) {
		cout<<"Error: getaddrinfo \n";
		exit(1);
	}	

	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);	
	cout<<"server: sockid:"<<sockfd<<endl;
	if(sockfd==-1) {
		cout<<"Error: socket \n";
		exit(1);
	}
	int yes=1;

	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) ==-1) {
		perror("setsockopt");
		exit(1);
	}

	bind(sockfd,res->ai_addr,res->ai_addrlen);
	while(1) {
		listen(sockfd,BACKLOG);

		infd = accept(sockfd,(struct sockaddr*)&inaddr,&addr_size);
		cout<<"sock:"<<infd<<endl;

		do {
			sleep(5);
			status = send(infd,"Hello\n",6,0);  
			if(status!=-1)
			{
				status =recv(infd,&buff,sizeof(buff),0);
				if(status==0 )
					break;
				else if(status==-1) {
					cout<<"Error:Unable to receive \n";
					break;
				}
				else {
					cout<<buff;
					memset(&buff,'\0',256);
				}
			}
		}while(status!=-1 && status !=0);
		
		close(infd);	
	}
	cout<<"Out of loop\n";
	
	close(sockfd);
	
	return 0;
}
