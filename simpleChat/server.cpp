// Chat-Server
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<forward_list>
#include "data.h"

using namespace std;
int sockfd,highfd;
fd_set readfds;
forward_list<int> clientids;

// Snd received mesg to all other clients
void handleMsg(int in_fd) {
	Message mesg;
	int result = recv(in_fd,&mesg,sizeof(mesg),0);
	if(result==0) {
		clients.remove(in_fd);
		close(in_fd);
	}
	else if(result>0) {
		for(auto i=clients.begin();i!=clients.end();i++) {
			if(*i!=in_fd) {
				send(*i,&mesg,sizeof(msg),0);
			}
		}
	}
	else if(result<0) {
		perror("recv");
	}
}

void handleNewConn() {
	int newfd;
	if((newfd = accept(sockfd,NULL,NULL))<0){
		perror("accept");
	}
	else {	
		clientids.push_front(newfd);
	}
}

void ListenConn() {
	if(FD_ISSET(sockfd,&readfds)) {
		handleNewConn();
	}
	else {
		for(auto i=clientids.begin();i!=clients.end();i++) {
			if(FD_ISSET(*i,&readfds)){
				handleMsg(*i);
			}
		}
	}
}

void SetSelectFDs() {
	FD_ZERO(&readfds);
	FD_SET(sockfd,&readfds);
	
	for(auto i=clientsids.begin();i!=clientids.end();i++)
		FD_SET(*i,&readfds);
}

int main() {
	struct addrinfo serv,*res;
	struct timeval tv;
	int on=1;

	FD_ZERO(&readfds);
	
	serv.ai_family = AF_UNSPEC;
	serv.ai_socktype = SOCK_STREAM;
	serv.ai_flags = AI_PASSIVE;

	if(getaddrinfo(NULL,MYPORT,&hints,&res) !=0) {
		perror("getaddrinfo");
	}
	
	if((sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol)) <0) {
		perror("socket");
	}
	
	setsockopt(sockfd,SOL_SOCKET,SO_REUSERADDR,&on,sizeof(int));
	FD_SET(sockfd,&readfds);

	if(bind(sockfd,res->ai_addr,res->ai_addrlen)<0) {
		perror("bind");
	}

	if(listen(sockfd,MCONN)<0) {
		perror("listen");
	}
	highfd = sockfd;

	while(1) {
		SetSelectFDs();
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		
		result = select(highfd+1,&readfds,NULL,NULL,&tv);
		if(result<1) {
			perror("select");
		}
		else if(result==0) {
			cout<<".";
		}
		else {
			ListenConn();
		}
	}

	return 0;
}
