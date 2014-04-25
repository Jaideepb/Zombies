// Desc: Infamous Producer-Consumer problem .
// Two processes, Producer :produces data, Consumer :consumes data.
// When the Queue is full, producer stops producing, and when Queue is empty Consumer stops consuming.
// Use FIFO to achieve interprocess communication.

#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>

#define FIFO_NAME "prod-cons"
using namespace std;

int main() {
	char s[30];	
	mknod(FIFO_NAME, S_IFIFO | 0666 ,0);
	cout<<"Waiting for consumers ...\n";

	int fd = open(FIFO_NAME,O_WRONLY);
	cout<<"got reader \n";

	int num;
	while(gets(s) , !feof(stdin)){
		cout<<s<<":"<<sizeof(s)<<endl;
		num=write(fd,s,strlen(s));
		if(num ==-1)
			perror("Write");
		else
			cout<<"produced "<<num<<" bytes"<<endl; 	
		sleep(3);
	}

	return 0;
}

