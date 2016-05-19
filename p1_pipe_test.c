/*
Write   a   program pipe_test which creates three   processes,  P1, P2, P3  
and pipes   as  needed.     The first       process displays    its pid,        prompts the user    for a   
string  and then    sends   it  to  the second  process.        The second  process     displays    its pid,    
displays    the string  it  received,   reverses    the string, displays    the reversed    string,     and 
sends   it  to  the third   process.        The third   process displays    its pid,    displays    the string      it  
received,       converts    the string  to  uppercase,  displays    the string  the string  it  
generated,  and sends       it  back    to  the first   process.        When    the first   process gets    the 
processed   string, it  displays    its pid and displays    the string  it  received    it  to  the 
terminal.       When    all this    is  done,   all processes   terminate       but display their   pid and a   
message announcing  their   imminent    death
*/

#include<ctype.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>



void reverse(char str[]){
    char *head = str;
    char *tail = str + strlen(str) - 1;
    char temp;

    while (head < tail){
        temp = *head;
        *head = *tail;
        *tail = temp;
        head++;
        tail--;
    }
}


// islower and toupper are basically functions to check and convert lower case to upper case

void cap(char str[]){
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if(islower(str[i])){
            str[i] = toupper(str[i]);
        }
    }
}


int main(int argc, char* argv[]){
    char buffer[500];
    int fd[2];                                  //file descriptor
    pid_t pid1 = 0;
    pid_t pid2 = 0;
    pid_t pid3 = 0;                     //parents, child, grandchild

    int status;
    
    printf("\nI am process P1. Nice to meet you!\n");

	//when main() is created, itself is a process already! it's a program, but also a parent process.

    pid1 = getpid();
    printf("My pid is %d\n", pid1);
    printf("Please enter a string: ");
    fgets(buffer, 500, stdin);                                      // stdin

    int i;
    for(i = 0; i < 500; i++) {
	if(buffer[i] == '\n') { buffer[i] = '\0'; }
    }


    pipe(fd);                                                       // Pipe
    
    
    status = pid1;		// pids are initialized to be 0 at the start. So it cannot be negative.
    if (pid2 < 0) {
        perror("fork() error");
        exit(-1);
    }
    
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork() error");
        exit(-1);
    }                                                           		//generate another process(child)

    if(pid2 != 0){								//this means that this is the parent process, the child process will
	    									//still have pid2 = 0.
        write(fd[1], buffer, 500);
        waitpid(pid2, &status, 0);
										// wait for its child process to complete before processing.
        read(fd[0], buffer, 500);
	printf("I am process P1\n");
	printf("My pid is %d\n", getpid());
        printf("I am about to die! Awwwwwwwwwwwwwwwwwww!\n\n");
        
    }
	
    else{                                                                      // Child process
        printf("\nI am process P2\n");
        pid2 = getpid();
        memset(buffer, 0, 500);							// Clear
        read(fd[0], buffer, 500);
	printf("I just received the string: %s from P1\n", buffer);
        reverse(buffer);
 	printf("I am sending ");
	printf("%s", buffer);
	printf(" to P3\n");
   	
    
        pid3 = fork();                                                          //generate grandchild process
        if (pid3 != 0){
            write(fd[1], buffer, 500);
            waitpid(pid3, &status, 0);
	    printf("I am process P2\n");
	    printf("My pid is %d\n", getpid());
            printf("I am still a child but I am about to die :( \n\n");
            
        }
	
	else{                                                                  //for n > 0
            printf("\nI am process P3\n");
            pid3 = getpid();
            memset(buffer, 0, 500);
            read(fd[0], buffer, 500);
	    printf("I just received the string: %s from P2\n", buffer);
            cap(buffer);
	    printf("I am sending ");
	    printf("%s", buffer);
	    printf(" to P1;\n\n");

            write(fd[1], buffer, 500);
            printf("I'm Process p3\nMy pid is %d\nI am still a grandchild but I am about to die :( :( \n\n", getpid());
        }
    }
    return 0;
}




















