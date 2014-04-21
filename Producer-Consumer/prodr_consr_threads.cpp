#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>

#define BUFF_MAX 5

using namespace std;

typedef struct queue {
	int buff[BUFF_MAX];
	int head,tail;
	int full,empty;
	pthread_mutex_t queue_m;
	pthread_cond_t  notfull,notempty;
}queue;

void* producer(void*);
void* consumer(void*);
void Qinit(queue &fifo_pc);

int main() {
	queue fifo_pc;
	pthread_t prod,cons;
	Qinit(fifo_pc);

	int ret1 = pthread_create(&prod,NULL,producer,&fifo_pc);
	int ret2 = pthread_create(&cons,NULL,consumer,&fifo_pc);

	pthread_join(prod,NULL);
	pthread_join(cons,NULL);

	return 0;
}

void* producer(void* arg){
	queue *fifo_pc=(queue*)arg;
	
	for(int i=0;i<20;i++) {
		pthread_mutex_lock(&fifo_pc->queue_m);
		if(fifo_pc->full) {
			pthread_cond_wait(&fifo_pc->notfull,&fifo_pc->queue_m);
		}	
		fifo_pc->buff[fifo_pc->tail++]=i;
		fifo_pc->empty=0;
		if(fifo_pc->tail==BUFF_MAX)
			fifo_pc->tail=0;
		if(fifo_pc->tail==fifo_pc->head)
			fifo_pc->full=1;
		printf("produced %d \n",i);
		pthread_mutex_unlock(&fifo_pc->queue_m);
		pthread_cond_signal(&fifo_pc->notempty);
	}	
}

void* consumer(void* arg){
	queue *fifo_pc=(queue*)arg;
	
	for(int i=0;i<20;i++) {
		pthread_mutex_lock(&fifo_pc->queue_m);
		if(fifo_pc->empty) {
			pthread_cond_wait(&fifo_pc->notempty,&fifo_pc->queue_m);
		}	
		cout<<fifo_pc->buff[fifo_pc->head++]<<endl;
		fifo_pc->full=0;
		if(fifo_pc->head==BUFF_MAX)
			fifo_pc->head=0;
		if(fifo_pc->tail==fifo_pc->head)
			fifo_pc->empty=1;
		pthread_mutex_unlock(&fifo_pc->queue_m);
		pthread_cond_signal(&fifo_pc->notfull);
		usleep(20000);
	}	
}

void Qinit(queue &fifo_pc) {
	fifo_pc.head=0;
	fifo_pc.tail=0;
	fifo_pc.full=0;
	fifo_pc.empty=1;
	pthread_mutex_init(&fifo_pc.queue_m,NULL);
	pthread_cond_init(&fifo_pc.notfull,NULL);
	pthread_cond_init(&fifo_pc.notempty,NULL);
}
