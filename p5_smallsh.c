/*
Extend  smallsh,  so  that  is  handles signals to  kill  processes.    Make  sure  
background  processes and the shell itself  are not killed  by  either  SIGINT or 
SIGQUIT.    Come  with  a creative  demonstration.    You might write a program that  is  
an  infinite  loop  and run if  first as  a foreground  process and then  in  the background.   
You should  be  able  to  kill  the foreground  process with  a signal  but not the 
background  one.    To  make  the demonstration more  interesting,  you could have  the 
action  of  SIGINT give the option  to  the user  to  change  his/her mind  about killing 
the foreground  process or  confirming  that  the process is  to  be  killed.   
*/

#include "smallsh.h" /* include file for example */
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>



/* program buffers and work pointers */
static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr = inpbuf, *tok = tokbuf;

userin(p) /* print prompt and read a line */
char *p;
{
  int c, count;
  /* initialization for later routines */
  ptr = inpbuf;
  tok = tokbuf;

  /* display prompt */
  printf("%s ", p);

  for(count = 0;;){
    if((c = getchar()) == EOF)
      return(EOF);

    if(count < MAXBUF)
      inpbuf[count++] = c;

    if(c == '\n' && count < MAXBUF){
      inpbuf[count] = '\0';
      return(count);
    }

    /* if line too long restart */
    if(c == '\n'){
      printf("smallsh: input line too long\n");
      count = 0;
      printf("%s ", p);
    }
  }
}

static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

inarg(c) /* are we in an ordinary argument */
char c;
{
  char *wrk;
  for(wrk = special; *wrk != '\0'; wrk++)
    if(c == *wrk)
      return(0);

  return(1);
}

gettok(outptr) /* get token and place into tokbuf */
char **outptr;
{
  int type;

  *outptr = tok;

  /* strip white space */
  for(;*ptr == ' ' || *ptr == '\t'; ptr++)
    ;

  *tok++ = *ptr;

  switch(*ptr++){
    case '\n':
      type = EOL; break;
    case '&':
      type = AMPERSAND; break;
    case ';':
      type = SEMICOLON; break;
    default:
      type = ARG;
      while(inarg(*ptr))
        *tok++ = *ptr++;
  }

  *tok++ = '\0';
  return(type);
}

/* execute a command with optional wait */

void sigaction_handler1(int sig){
    int option;
    printf("HAHAHAHAHAHAHHAHAHA, I got a signal to terminate it!!\n");
    //printf("Really? press 1 to terminate, presee 2 to continue to run \n");
    //scanf("%d",&option);
    //if (option ==1 ){
       //printf("asdfasdfdfdfs\n");
    /*   return 1;

    }
    else{
      
      return 0;
    }*/
  }

  void sigaction_handler2(int sig){
    printf("inside handler2");
  }


runcommand(cline, where)
char **cline;
int where;
{
  //add if statement and else according to c line
  int pid, exitstat, ret;
  struct sigaction act;
  if((act.sa_handler=sigaction_handler1)==0){
        act.sa_handler=sigaction_handler2;
     }
  else{
    act.sa_handler=sigaction_handler1;
  }
  //if (act.sa_handler == 1){
  //  act.sa_handler= SIG_IGN;
  //}



  if((pid = fork()) < 0){
    perror("smallsh");
    return(-1);
  }

  if(pid == 0){
    //signal(SIGINT,alrmhandle);
    //signal(SIGQUIT,alrmhandle);
    execvp(*cline, cline);
    perror(*cline);
    exit(127);
  }

  /* code for parent */
  /* if background process print pid and exit */
  if(where == BACKGROUND){
    printf("[Process id %d]\n", pid);
    return(0);
  }
  

  if (where == FOREGROUND){//if it is foreground and we noticed kill we kill
          sigaction(SIGINT,&act,NULL);
          sigaction(SIGQUIT,&act,NULL);
  }

  //sigaction(SIGINT,&act,NULL);
  //sigaction(SIGQUIT,&act,NULL);


  /* wait until process pid exits */
  while( (ret=wait(&exitstat)) != pid && ret != -1)
    ;

  return(ret == -1 ? -1 : exitstat);
}

procline() /* process input line */
{
  char *arg[MAXARG+1]; /* pointer array for runcommand */
  int toktype; /* type of token in command */
  int narg; /* numer of arguments so far */
  int type; /* FOREGROUND or BACKGROUND? */

  for(narg = 0;;){ /* loop forever */
    /* take action according to token type */
    switch(toktype = gettok(&arg[narg])){
      case ARG:
        if(narg < MAXARG)
          narg++;
        break;

      case EOL:
        //printf("EOL\n");
        //type = (toktype == EOL) ? BACKGROUND : FOREGROUND;
        //break;

      case SEMICOLON:
        //printf("SC\n");
        //type = (toktype == SEMICOLON) ? FOREGROUND : BACKGROUND;
        //break;

      case AMPERSAND:
        type = (toktype == AMPERSAND) ? BACKGROUND : FOREGROUND;

        if(narg != 0){
          arg[narg] = NULL;
          runcommand(arg, type);
        }

        if(toktype == EOL)
          return;

        narg = 0;
        break;
    }
  }
}

char *prompt = "Command>"; /* prompt */

main()
{
  while(userin(prompt) != EOF)
    procline();
}

