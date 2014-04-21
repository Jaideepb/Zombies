#include<iostream>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

#define FIFO_NAME "prodcons_th"

using namespace std;
int buffer =4;
pthread_mutex_t stdout_t;

void* producer(void* arg) {
	mknod(FIFO_NAME,S_IFIFO|0666,0); 
	pthread_mutex_lock(&stdout_t);
	cout<<"Waiting for consumer .... "<<endl;
	pthread_mutex_unlock(&stdout_t);

	int fd=open(FIFO_NAME,O_WRONLY);

	pthread_mutex_lock(&stdout_t);
	cout<<"got consumer "<<endl;
	pthread_mutex_unlock(&stdout_t);
	char s[30];
	while( gets(s), !feof(stdin)) {
		int ret=write(fd,s,strlen(s));
		if(ret==-1) {
			perror("write");
		}			
		else {
			pthread_mutex_lock(&stdout_t);
			cout<<"produced "<<ret<<" bytes"<<endl;
			pthread_mutex_unlock(&stdout_t);
		}
	} 	
}

void* consumer(void *arg) {
	mknod(FIFO_NAME,S_IFIFO|0666,0);

	pthread_mutex_lock(&stdout_t);
	cout<<"Waiting for producer ... "<<endl;
	pthread_mutex_unlock(&stdout_t);
	
	int fd=open(FIFO_NAME,O_RDONLY);
	pthread_mutex_lock(&stdout_t);
	cout<<"got producer "<<endl;
	pthread_mutex_unlock(&stdout_t);

	char s[30];
	int ret;	
	do{
		ret=read(fd,s,sizeof(s));
		if(ret==-1)
			perror("read");
		else {
			s[ret]='\0';
			
			pthread_mutex_lock(&stdout_t);
			printf("consumed %s \n",s);
			pthread_mutex_unlock(&stdout_t);
		}

	}while(ret>0);
}

int main() {
	pthread_t prod,cons;
	pthread_mutex_init(&stdout_t,NULL);
	int ret1 =pthread_create(&prod,NULL,producer,NULL);
	if(ret1){
		exit(-1);
	}
	int ret2 =pthread_create(&cons,NULL,consumer,NULL);
	if(ret2) {
		exit(-1);
	}

	pthread_join(prod,NULL);
	pthread_join(cons,NULL);	

	return 0;
}
