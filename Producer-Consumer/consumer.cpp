#include<iostream>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define FIFO_NAME "prod-cons"

using namespace std;

int main() {
	
	mknod(FIFO_NAME,S_IFIFO |0666, 0);
	cout<<"Waiting for producers ... "<<endl;

	int fd=open(FIFO_NAME,O_RDONLY);		
	cout<<"got producer "<<endl;
	char ss[30];
	int num=0;
	do {
		num=read(fd,ss,sizeof(ss));
		if(num==-1)
			perror("read");
		ss[num]='\0';
		printf("%s\n",ss);

	}while(num>0);		


	return 0;
}

