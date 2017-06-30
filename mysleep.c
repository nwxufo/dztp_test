/*
 *  mysleep.c
 *  Author: milo
 *  Mail: printer_bzu@163.com 
 *  Created Time: 2017-01-17 11:10
 */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_alrm( int *signo)
{
	/* nothing to do */
	printf("1");
}

/*
 *	struct sigaction {
 *		void 		(*sa_handler)(int);
 *		sigset_t 	sa_mask;
 *		int		sa_flags;
 *		void		(*sa_sigaction)(int,siginfo_t *, void *);
 *	}
 */
unsigned int mysleep(unsigned int nsecs)
{
	struct sigaction newact, oldact;
	unsigned int unslept;

	newact.sa_handler =(void*) sig_alrm;
	sigemptyset( &newact.sa_mask );
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);

	alarm(nsecs);
	pause();

	unslept = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);

	return unslept;
}

int main(int argc, char* argvs[])
{
	while(1){
		mysleep(2);
		printf("Two seconds passed\n");
	}

	return 0;
}
