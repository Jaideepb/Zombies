#include<sys/types.h>
#include<sys/time.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>


#define STDIN 0 
using namespace std;

int sockid;
int clientfds[5];	
fd_set readfds;
struct sockaddr_storage inaddr;
socklen_t addr_size;

void ListenInConn(int num) {
	//Check for new connection
	if(FD_ISSET(sockid,&readfds)) {
		// accept(sockid,&inaddr,&addr_size);
		int fdtemp = accept(sockid,(struct sockaddr*)&inaddr,&addr_size);
		if(fdtemp<0) {
			perror("accept ");
			exit(-1);
		}
		for(int i=0;i<5;i++) {
			if(clientfds[i] == 0) {
				clientfds[i]=fdtemp;
				break;
			}
		}
		
		if(i==5)
			cout<<"Conn buff full"<<endl;
		else 
			ReceiveData(fedtemp);
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
	
	tv.tv_sec =2;
	tv.tv_usec = 500000;
	
	struct addrinfo hint,*res;

	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE; 	

	//getaddrinfo(website,port,addrinfo &their,addinfo &res)
	getaddrinfo(NULL,MYPORT,&hint,&res);
	//socket(family,type,protocol)
	sockid = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	//bind(sockid,addrinfo.ai_addr,addrlen);
	bind(sockid,res->ai_addr,res->ai_addrlen);
	
	FD_SET(sockid,&readfds);
	for(int i=1-1,i<5;i++) {
		clientfds[i]=sockid+i+1;
		FD_SET(clientfds[i],&readfds);
	}

	while(1) {
		res = select(sockid+6,&readfds,NULL,NULL,&tv);
		if(res<0) {
			perror("select");
		}
		else if(res>0) {
			ListenInConn(res);
		}
	}

	return 0;
}

