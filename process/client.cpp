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
#include<unistd.h>
#include<fcntl.h>

#define PORT "4345"

using namespace std;

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

int main() {
	struct addrinfo hint,*res;
	int sockfd,status;
	char buff[256],outbuff[256];
	//memset(outbuff,'0',256);
	memset(&hint,0,sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;

	if((status=getaddrinfo("127.0.0.1",PORT,&hint,&res))!=0){
		cout<<"Error: getaddrinfo\n";
		exit(-1);
	}

	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
//	setnonblocking(sockfd);

	cout<<"client: sockid:"<<sockfd<<endl;

	if(sockfd==-1) {
		cout<<"Error: socket \n";
		exit(1);
	}

	int count=0;
	if(connect(sockfd,res->ai_addr,res->ai_addrlen)!=-1) {
		//sleep(1);
		recv(sockfd,&buff,256,0);
		if(send(sockfd,"Hello",6,0)<0) {
			perror("Send");
		}	
		memset(outbuff,'\0',sizeof(outbuff));
		snprintf(outbuff,sizeof(outbuff),"%d\n",count);
		
		while (count<10) {
		//	sleep(1);
			if((status = send(sockfd,outbuff,sizeof(outbuff),0)) >0 );

			else 
				perror("send");

			count++;
			memset(outbuff,'\0',sizeof(outbuff));
			snprintf(outbuff,sizeof(outbuff),"%d\n",count);
		//	recv(sockfd,&buff,256,0);
		//	cout<<buff;
		//	memset(buff,'\0',sizeof(buff));
		}
		cout<<count<<endl;
/*		if(count<10) {
			memset(buff,0,sizeof(buff));
			memset(outbuff,'0',256);
			if((status = recv(sockfd,&buff,256,0)) >0 )
				cout<<buff<<endl;
			else
				perror("recv");

			count++;
			sleep(6);
			snprintf(outbuff,sizeof(outbuff),"%d\n",count);
//			cout<<"client: "<<outbuff<<endl;
			if(send(sockfd,"second-Msg",10,0)<=0)
				perror("send");
		}
*/
	}
	else 
		cout<<"Error: connect \n";
	
	close(sockfd);
	
	return 0;
}

