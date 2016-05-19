//Can Cheng, Keith Liu, Chen Chen
/*
Write	a	program	process_tree that	takes	a	single	integer	argument	
n from	the	command	line	and	creates	a	binary	tree	of	processes	of	depth	n. When	
the	tree	is	created,	each	process,	each	process	should	display	the	phrase	
			I am process x; my process pid is p (the	pid	for	process	x);
				My parents pid is p1 (the	pid	for	this	process’	parent)
After	printing	as	indicated	above,	the	process	terminates.
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int depth = 0;
    int i;
    int status;
    int depth_count=0;
    printf("tree ");
    int id=1;//keeps track of the process id
    int lid=0, rid=0;
    scanf("%d", &depth);
   // printf(" depth is %d ", depth);
    printf("I am process 1 ; my process pid is %d \n", getpid() );
    printf("My parents pid is %d", getppid());
    for (i =0; i <depth-1;i++){
        int lpid = fork();
	
        //printf("lpid is %d \n", lpid);
	if (lpid == 0){
		lid=2*id;
		int current_pid=getpid();
		int parent_pid=getppid();
		printf("I am process %d ; my process pid is %d \n", lid, current_pid);		
		printf("My parents pid is %d \n", parent_pid);
                id=lid;
         }
	else{//we are not inside left child
		//printf("hehrehr\n");
		int rpid = fork();
		//printf("rpid is %d \n",rpid);
		if (rpid == 0){//at right child
			rid=2*id+1;
                	int current_pid1=getpid();
                	int parent_pid1=getppid();
               		printf("I am process %d ; my process pid is %d \n", rid, current_pid1);
               		printf("My parents pid is %d \n", parent_pid1);
                	id=rid;
		}
	else
		 {
		 	//printf("I am process 1 ; my process pid is %d \n" , getpid());		
			//printf("My parents pid is %d \n", getppid());	
			status=waitpid(-1,NULL,0);
			while (status !=-1){
			status=waitpid(-1,NULL,0);
                        }
			break;
		 } 
	}   
    }//for loop

   return 0;
}
