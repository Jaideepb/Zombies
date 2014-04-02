#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<errno.h>
#include "data.h"


using namespace std;
int sockfd;
fd_set readfds;

int main(int argc,char *argv[]) {
	/*
	if(argc<3) {
		cout<<"Usage ./client <Username> <Password>\n";
		exit(1);
	}

	string uName(argv[1]);	// Username
	string uPass(argv[2]);	// Password
	
	 */

	int logopt;
	int result;
	struct addrinfo hints,*res;
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	result=getaddrinfo("127.0.0.1","2345",&hints,&res);

	if(result) {
		if (result == EAI_SYSTEM)
	        fprintf(stderr, "looking up www.example.com: %s\n", strerror(errno));
	    else
        	fprintf(stderr, "looking up www.example.com: %s\n", gai_strerror(result));
		    return -1;
	}
	
	sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

	if(sockfd<0) {
		perror("socket");
		exit(1);
	}
/*	
	if((result =connect(sockfd,res->ai_addr,res->ai_addrlen))<0) {
		perror("connect");
		exit(1);
	}
*/
/*	cout<<"Enter Option: 1: NewUser-Register 2:Login => ";
	cin>>logopt;
	
	if(logopt==REGISTER) {
		logStatus=REGISTER;

		while(logStatus==REGISTER) {
			struct 
			cout<<"Username : ";
			cin>>uName;
			cout<<"Password : ";
			cin>>uPass;	
			
			send(
	}
*/

	string str;

	int xyz,resl;
	while(1) {
		
		FD_ZERO(&readfds);
		FD_SET(0,&readfds);
		resl = select(3,&readfds,NULL,NULL,NULL);

		if(resl<0) {
			perror("Select");
			exit(1);
		}
		else if(resl>0) {
			cout<<"HERE";
			cin>>xyz;
			cout<<xyz<<endl;
		}

	//	cin>>str;		
		
	//	if(send(sockfd,str.c_str(),str.length(),0) <0)
	//		perror("Send");


	}

	return 0;
}

