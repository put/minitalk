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

#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include "../shared.h"

int	register_client(void)
{
	int	tries;

	tries = 0;
	clientinfo()->state = WAIT_INIT_CONF;
	while (clientinfo()->state == WAIT_INIT_CONF && tries < 5000)
	{
		kill(clientinfo()->serv_pid, SIGUSR1);
		usleep(1000);
		tries++;
	}
	if (clientinfo()->state == WAIT_INIT_CONF)
		return (1);
	return (0);
}

void	transmit_int(int value)
{
	int	bitcount;
	int	bit;

	bitcount = 31;
	while (bitcount >= 0)
	{
		clientinfo()->state = WAIT_INT_CONF;
		bit = (value >> bitcount) & 1;
		if (bit == 0)
			kill(clientinfo()->serv_pid, SIGUSR1);
		else
			kill(clientinfo()->serv_pid, SIGUSR2);
		bitcount--;
		while (clientinfo()->state == WAIT_INT_CONF)
			pause();
	}
}

void	transmit_str(int str_len, char *str)
{
	int	charcount;
	int	bitcount;
	int	bit;

	charcount = 0;
	while (charcount < str_len)
	{
		bitcount = 7;
		while (bitcount >= 0)
		{
			clientinfo()->state = WAIT_STR_CONF;
			bit = (str[charcount] >> bitcount) & 1;
			if (bit == 0)
				kill(clientinfo()->serv_pid, SIGUSR1);
			else
				kill(clientinfo()->serv_pid, SIGUSR2);
			bitcount--;
			while (clientinfo()->state == WAIT_STR_CONF)
				pause();
		}
		charcount++;
	}
}

void	handle_sigusr(int signo, siginfo_t *siginf, void *context)
{
	(void)context;
	if (siginf->si_pid != clientinfo()->serv_pid)
		return ;
	if (signo == SIGUSR1)
		clientinfo()->state = IDLE;
}

int	main(int argc, char **argv)
{
	int	pid;
	int	str_len;

	if (argc != 3)
		return (ft_printf("Usage: ./client <server pid> <string>\n"), 1);
	pid = atoi(argv[1]);
	if (pid < 1)
		return (ft_printf("Invalid PID provided\n"), 1);
	str_len = strlen(argv[2]);
	if (str_len < 1)
		return (ft_printf("Empty string provided, nothing to send\n"), 1);
	clientinfo()->serv_pid = pid;
	if (register_handlers(&handle_sigusr))
		return (ft_printf("Failed to register handlers\n"), 1);
	if (register_client())
		return (ft_printf("Failed to register client, timed out\n"), 1);
	transmit_int(str_len);
	transmit_str(str_len, argv[2]);
	return (0);
}
