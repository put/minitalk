/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   clientmain.c									   :+:	:+:			*/
/*													 +:+					*/
/*   By: mika <mika@student.codam.nl>				 +#+					 */
/*												   +#+					  */
/*   Created: 2025/02/06 17:06:54 by mika		  #+#	#+#				 */
/*   Updated: 2025/02/06 17:20:23 by mika		  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#define _XOPEN_SOURCE 700 // lets hope this is not required (or is allowed) at codam
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "client.h"

static volatile clientinfo_t info = (clientinfo_t){0, IDLE};


long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

void register_client()
{
	info.state = WAIT_INIT_CONF;
	kill(info.serv_pid, SIGUSR1);
	while (info.state == WAIT_INIT_CONF)
		pause();
}

void transmit_int(int value)
{
	int bitcount;
	int bit;

	bitcount = 31;
	while (bitcount >= 0)
	{
		info.state = WAIT_INT_CONF;
		bit = (value >> bitcount) & 1;
		if (bit == 0)
			kill(info.serv_pid, SIGUSR1);
		else
			kill(info.serv_pid, SIGUSR2);
		bitcount--;
		while (info.state == WAIT_INT_CONF)
			pause();
	}
}

void transmit_str(int str_len, char *str)
{
	int charcount;
	int bitcount;
	int bit;

	charcount = 0;
	while (charcount < str_len)
	{
		bitcount = 7;
		while (bitcount >= 0)
		{
			info.state = WAIT_STR_CONF;
			bit = (str[charcount] >> bitcount) & 1;
			if (bit == 0)
				kill(info.serv_pid, SIGUSR1);
			else
				kill(info.serv_pid, SIGUSR2);
			bitcount--;
			while (info.state == WAIT_STR_CONF)
				pause();
		}
		charcount++;
	}
}

void handle_sigusr(int signo, siginfo_t *siginf, void *context)
{
	if (siginf->si_pid != info.serv_pid)
		return ;
	info.state = IDLE;
}

int register_handlers()
{
	struct sigaction sa;
	sa.sa_sigaction = &handle_sigusr;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	return (sigaction(SIGUSR1, &sa, NULL)
		+ sigaction(SIGUSR2, &sa, NULL));
}

int main(int argc, char **argv)
{
	int pid;
	int str_len;
	int bit;
	long long starttime;
	long long endtime;

	if (argc != 3)
		return(printf("Usage: ./client <server pid> <string>\n"), 1);
	pid = atoi(argv[1]);
	if (pid < 1)
		return(printf("Invalid PID provided\n"), 1);
	str_len = strlen(argv[2]);
	if (str_len < 1)
		return(printf("Empty string provided, nothing to send\n"), 1);
	info.serv_pid = pid;
	if (register_handlers())
		return (printf("Failed to register handlers\n"), 1);
	starttime = timeInMilliseconds();
	register_client();
	printf("Registered client!\n");
	transmit_int(str_len);
	printf("Finished transmitting strlen!\n");
	transmit_str(str_len, argv[2]);
	printf("Finished transmitting string in %lldms!\n", timeInMilliseconds() - starttime);
	return (0);
}
