#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

/*-------------------------------------------------------------------------*/
void sigint_handler(int sig)
{
    write(0, "Ahhh! SIGINT!\n", 14);   //write to stdout   - async-safe (not like printf)
}
/*-------------------------------------------------------------------------*/

int main(void)
{
    void sigint_handler(int sig); /* prototype */
    char s[200];
    struct sigaction sa;

	/*-------------------------------------*/
	/*initialize sig action*/
    sa.sa_handler = sigint_handler;     /*handler function address*/
    /*sa.sa_flags = 0; // or SA_RESTART*/
	sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
	/*-------------------------------------*/
	/*replace signal handler number SIGINT (ctrl+c) in current process signal table*/
    if (sigaction(SIGINT, &sa, NULL) == -1) {   
        perror("sigaction");
        exit(1);
    }

	/*-------------------------------------*/
    printf("Enter a string:\n");

    if (fgets(s, sizeof s, stdin) == NULL)
        perror("fgetsXXX");
    else 
        printf("You entered: %s\n", s);
	/*-------------------------------------*/


    return 0;
}

/*-------------------------------------------------------------------------*/

/* async safe functions
_Exit(), _exit(), abort(), accept(), access(), aio_error(), aio_return(), aio_suspend(), alarm(), bind(), cfgetispeed(), cfgetospeed(), cfsetispeed(), cfsetospeed(), chdir(), chmod(), chown(), clock_gettime(), close(), connect(), creat(), dup(), dup2(), execle(), execve(), fchmod(), fchown(), fcntl(), fdatasync(), fork(), fpathconf(), fstat(), fsync(), ftruncate(), getegid(), geteuid(), getgid(), getgroups(), getpeername(), getpgrp(), getpid(), getppid(), getsockname(), getsockopt(), getuid(), kill(), link(), listen(), lseek(), lstat(), mkdir(), mkfifo(), open(), pathconf(), pause(), pipe(), poll(), posix_trace_event(), pselect(), raise(), read(), readlink(), recv(), recvfrom(), recvmsg(), rename(), rmdir(), select(), sem_post(), send(), sendmsg(), sendto(), setgid(), setpgid(), setsid(), setsockopt(), setuid(), shutdown(), sigaction(), sigaddset(), sigdelset(), sigemptyset(), sigfillset(), sigismember(), sleep(), signal(), sigpause(), sigpending(), sigprocmask(), sigqueue(), sigset(), sigsuspend(), sockatmark(), socket(), socketpair(), stat(), symlink(), sysconf(), tcdrain(), tcflow(), tcflush(), tcgetattr(), tcgetpgrp(), tcsendbreak(), tcsetattr(), tcsetpgrp(), time(), timer_getoverrun(), timer_gettime(), timer_settime(), times(), umask(), uname(), unlink(), utime(), wait(), waitpid(), and write().
*/


/*can call your own functions from within your signal handler (as long they don't call any non-async-safe functions.)*/

/* cannot safely alter any shared (e.g. global) data, with one notable exception: variables that are declared to be of storage class and type volatile sig_atomic_t.*/






