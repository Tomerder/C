#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

/*kill -USR1 5023*/

/*-------------------------------------------------------------------------*/
/* cannot safely alter any shared (e.g. global) data, with one notable exception: variables that are declared to be of storage class and type volatile sig_atomic_t.*/

volatile sig_atomic_t got_usr1; 
/*-------------------------------------------------------------------------*/
void sigusr1_handler(int sig)
{
    got_usr1 = 1;  //sets global volatile sig_atomic_t to TRUE => when process is killed  
}
/*-------------------------------------------------------------------------*/
int main(void)
{
    struct sigaction sa;

    got_usr1 = 0;

	/*-------------------------------------*/
	/*initialize sig action*/
    sa.sa_handler = sigusr1_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	/*set sig action SIGUSR1 = kill*/
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

	/*-------------------------------------*/
    while (!got_usr1) {  //flag will be true when process is killed
        printf("PID %d: working hard...\n", getpid());
        sleep(1);  //sleep() gets interrupted by signals.
    }

    printf("Done in by SIGUSR1!\n");
	/*-------------------------------------*/

    return 0;
}


/*
Signal 	Description
SIGABRT 	Process abort signal.
SIGALRM 	Alarm clock.
SIGFPE 		Erroneous arithmetic operation.
SIGHUP 		Hangup.
SIGILL 		Illegal instruction.
SIGINT 		Terminal interrupt signal.
SIGKILL 	Kill (cannot be caught or ignored).
SIGPIPE 	Write on a pipe with no one to read it.
SIGQUIT 	Terminal quit signal.
SIGSEGV 	Invalid memory reference.
SIGTERM 	Termination signal.
SIGUSR1 	User-defined signal 1.
SIGUSR2 	User-defined signal 2.
SIGCHLD 	Child process terminated or stopped.
SIGCONT 	Continue executing, if stopped.
SIGSTOP 	Stop executing (cannot be caught or ignored).
SIGTSTP 	Terminal stop signal.
SIGTTIN 	Background process attempting read.
SIGTTOU 	Background process attempting write.
SIGBUS 		Bus error.
SIGPOLL 	Pollable event.
SIGPROF 	Profiling timer expired.
SIGSYS 		Bad system call.
SIGTRAP 	Trace/breakpoint trap.
SIGURG 		High bandwidth data is available at a socket.
SIGVTALRM 	Virtual timer expired.
SIGXCPU 	CPU time limit exceeded.
SIGXFSZ 	File size limit exceeded.
*/















