
#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

using namespace std;

int main(){
	pid_t pID;
	
	pID = fork();
	switch(pID) {
	
		case 0: 
			cout<<"Child process \n";
			exit(4);
			break;
		case -1:
			cout<<"Error:Fork\n";
			break;
		default:
			int childExitStatus;
			waitpid(pID,&childExitStatus,0);
			cout<<childExitStatus<<endl;
			cout<<"Parent \n";	
			break;
	}
	
	return 0;
}	
