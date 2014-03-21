#include<sys/types.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<errno.h>
#include<unistd.h>
#include<iostream>
#include<string.h>
#include<fcntl.h>

#define STDIN 0 
#define MYPORT "4345"
using namespace std;

int sockid;
int clientfds[5];	
fd_set readfds;
struct sockaddr_storage inaddr;
socklen_t addr_size;

void setnonblocking(int fd) {
	int opts;
	
	opts= fcntl(fd,F_GETFL);
	if(opts <0) {
		perror("fcntl");
		exit(EXIT_FAILURE);
	}	
	opts = (opts | O_NONBLOCK);
	if(fcntl(fd,F_SETFL,opts)<0) {
		perror("fcntl-set");
		exit(EXIT_FAILURE);
	}
	return;
}

void ReceiveData(int idx) {
	int fd=clientfds[idx];
	char buff[256];
	if(recv(fd,&buff,256,0)>0) {
		cout<<buff;
		send(fd,"Received!",9,0);
		memset(buff,0,sizeof(buff));
	}
	else {
		close(fd);
		clientfds[idx]=0;
	}
}

void ListenInConn(int num) {
	//Check for new connection
	if(FD_ISSET(sockid,&readfds)) {
		// accept(sockid,&inaddr,&addr_size);
		int fdtemp = accept(sockid,(struct sockaddr*)&inaddr,&addr_size);
		if(fdtemp<0) {
			perror("accept ");
			exit(-1);
		}
		int i;
		for(i=0;i<5;i++) {
			if(clientfds[i] == 0) {
				clientfds[i]=fdtemp;
				break;
			}
		}
		
		if(i==5)
			cout<<"Conn buff full"<<endl;
		else 
			ReceiveData(i);
	}
	
	//Check for data on existing connections
	for(int i=0;i<5;i++) {
		if(FD_ISSET(clientfds[i],&readfds)) {
			ReceiveData(clientfds[i]);
		}
	}
}


int main() {
	struct timeval tv;
	fd_set writefds;
	int result;
	struct addrinfo hint,*res;

	memset(&hint,0,sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE; 	
	
	memset(clientfds,0,sizeof(clientfds));

	//getaddrinfo(website,port,addrinfo &their,addinfo &res)
	if((result=getaddrinfo(NULL,MYPORT,&hint,&res))!=0) {
		cout<<result;
	//	perror("getaddrinfo");
	}
	cout<<"Here"<<endl;
	//socket(family,type,protocol)
	if ((sockid=socket(res->ai_family,res->ai_socktype,res->ai_protocol))<0) {
		perror("Socket");
	}
	cout<<sockid<<endl;
	
	int yes =1;
	setsockopt(sockid,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	setnonblocking(sockid);
	setsockopt(sockid,IPPROTO_IPV6,IPV6_V6ONLY,(char *)&yes,sizeof(yes));
	//bind(sockid,addrinfo.ai_addr,addrlen);
	if(bind(sockid,res->ai_addr,res->ai_addrlen)<0){
		perror("Bind");
	}

	while(1) {
		FD_ZERO(&readfds);	
		FD_SET(sockid,&readfds);

		tv.tv_sec =4;
		tv.tv_usec = 500000;

		result=0;
		result = select(sockid+1,&readfds,NULL,(fd_set *) 0,&tv);
		cout<<"result= "<<result<<endl;
		if(result<0) {
			perror("select");
		}
		else if(result>0) {
			ListenInConn(result);
		}
	}

	return 0;
}

