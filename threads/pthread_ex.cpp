
#include<iostream>
#include<pthread.h>
#include<stdio.h>

using namespace std;
long wat = 0;
pthread_mutex_t mut;

void * print_message_func(void *ptr) {
	int *msg;
	msg = (int*)ptr;
	pthread_mutex_lock(&mut);
	cout<<pthread_self()<<":HERE "<<wat<<endl;
	for(long i=0;i<1000000;i++) {
		wat = wat+ 1;
	}
	cout<<pthread_self()<<":End "<<wat<<endl;
	
	pthread_mutex_unlock(&mut);
}

int main() {
	pthread_t th1,th2,th3;
//	const char *msg1="Thread 1";
	int msg1[2]={1,2};
//	const char *msg2="Thread 2";
	int msg2[2]={3,4};
	int msg3[2]={5,6};
	int iret1,iret2,iret3;

	iret1 = pthread_create(&th1,NULL,print_message_func, (void*)msg1);
	iret2 = pthread_create(&th2,NULL,print_message_func, (void*)msg2);
	iret3 = pthread_create(&th3,NULL,print_message_func, (void*)msg3);
	cout<<th1<<"  "<<th2<<"  "<<th3<<endl;
	pthread_join(th1,NULL);
	cout<<"Over 1"<<endl;
	pthread_join(th2,NULL);
	cout<<"Over 2"<<endl;
	pthread_join(th3,NULL);
	cout<<"Over 3"<<endl;

	return 0;
}
