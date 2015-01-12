#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
 //~ sigset_t old_set,new_set;
 //~ sigemptyset(&old_set);
 //~ sigemptyset(&new_set);
//~ 
 //~ if(sigaddset(&old_set,SIGSEGV)==0)
 //~ {
  //~ printf("sigaddset successfully added for SIGSEGV\n");
 //~ }
 //~ sigprocmask(SIG_BLOCK,&new_set,&old_set); // SIGSEGV signal is masked
 //~ kill(0,SIGSEGV);
//~ 
//~ 
//~ //*****************************************************************
//~ 
//~ if(sigaddset(&new_set,SIGRTMIN)==0)
//~ {
  //~ printf("sigaddset successfully added for SIGRTMIN\n");
//~ }
//~ if(sigprocmask(SIG_BLOCK,&new_set,&old_set)==-1) // SIGRTMIN signal is masked
//~ {
  //~ perror("sigprocmask");
//~ }
//~ kill(0,SIGSEGV);
//~ 
//~ 
//~ //****************** Unblock all signals  ******************
//~ 
//~ if(sigprocmask(SIG_UNBLOCK,&new_set,&old_set)==-1) // SIGRTMIN signal is unmasked
 //~ {
  //~ perror("sigprocmask");
 //~ }

 for(;;);
}
