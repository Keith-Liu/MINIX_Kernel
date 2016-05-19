//CAN CHENG ,CHEN CHEN, KEITH LIU
/*
	Write	a	program	AlrmTest that does	the	following.		Creates	two	child	
processes	that	each	increments	a	process-local	variable	count in	an infinite	loop.				
The	parent	is	just	 a	for loop	of	5	iterations,	where	in each	interation it sleeps	for	a	
second	and	then	sends	the	SIGALRM signal	to	each	child.			Upon	receiving	this	
signal,	each	child	is	to	print	out	the	value	of	count along	with	its	process	id	and	
reset	the	count	to	zero.	
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>



int count = 0;
void handler(int signumber)
{

	printf("Count is %d, my pid is %d\n",count,getpid());
    count = 0;
}
int main()
{
	//struct sigaction action;
	int i;
	int parent_pid;
	int pid1,pid2;
	int childpid;

	if((childpid = fork()) < 0)
	{
		printf("Fork Error");
		return -1;
	}

	if(childpid != 0)//parent
	{
		if((childpid = fork()) < 0)
		{
			printf("Fork Error");
			return -1;
		}
		if(childpid == 0) //in the child2 process
		{//set up alarm handler here
			signal(SIGALRM,handler);

			while(1)
			{
				count++;
            }
		}
		else//parent
		{
			parent_pid = getpid();
			pid1=parent_pid+1;
			pid2=parent_pid+2;

			for(i = 1; i <= 5; i++)//run 5 times as desired
			{
				sleep(1);//sleep for a sec
				kill(pid1,SIGALRM);//send signal to pid1 to wake it
				kill(pid2,SIGALRM);//same as above
			}
		}
	}
	else //child1 process
	{//set up the alarm handler
		signal(SIGALRM,handler);
		while(1)
		{
			count++;
		}
	}

}
