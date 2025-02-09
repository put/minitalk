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
#include "client.h"
#include "../shared.h"

void	send_sig(int pid, int sig, t_clientstate success_state)
{
	int	attempts;
	
	attempts = 0;
	clientinfo()->state = WAIT_SEND;
	ft_printf("send_sig called from PID %d\n", getpid());
	while (clientinfo()-> state == WAIT_SEND && attempts < 500)
	{
		kill(pid, sig);
		usleep(10000);
		attempts++;
		if (clientinfo()->state != WAIT_SEND)
    		break;
	}
	if (attempts < 500)
		clientinfo()->state = success_state;
	else
		clientinfo()->state = TIMEOUT;
}

int	transmit_int(int value)
{
	int	bitcount;
	int	bit;

	bitcount = 31;
	while (bitcount >= 0)
	{
		bit = (value >> bitcount) & 1;
		send_sig(clientinfo()->serv_pid, bit_to_sig(bit), SENDING_INT);
		if (clientinfo()->state == TIMEOUT)
			return (0);
		bitcount--;
	}
	return (1);
}

int	transmit_str(int str_len, char *str)
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
			bit = (str[charcount] >> bitcount) & 1;
			send_sig(clientinfo()->serv_pid, bit_to_sig(bit), SENDING_STR);
			if (clientinfo()->state == TIMEOUT)
				return (0);
			bitcount--;
		}
		charcount++;
	}
	return (1);
}

void	handle_sigusr(int signo, siginfo_t *siginf, void *context)
{
	(void)context;
	if (siginf->si_pid != clientinfo()->serv_pid)
		return ;
	ft_printf("Received signal from server on %d\n", getpid());
	if (signo == SIGUSR1)
		clientinfo()->state = IDLE;
	if (signo == SIGUSR2)
		clientinfo()->state = FAIL;
}

int	main(int argc, char **argv)
{
	int	pid;
	int	str_len;

	if (argc != 3)
		return (ft_printf("Usage: ./client <server pid> <string>\n"), 1);
	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (ft_printf("Invalid PID provided\n"), 1);
	str_len = (int)ft_strlen(argv[2]);
	if (str_len < 1)
		return (ft_printf("Empty string provided, nothing to send\n"), 1);
	clientinfo()->serv_pid = pid;
	if (register_handlers(&handle_sigusr))
		return (ft_printf("Failed to register handlers\n"), 1);
	send_sig(clientinfo()->serv_pid, SIGUSR1, INIT_DONE);
	if (clientinfo()->state == TIMEOUT)
		return (ft_printf("Failed to register client, timed out\n"), 1);
	if(!transmit_int(str_len))
		return (ft_printf("Failed to transmit int, timed out\n"), 1);
	if(!transmit_str(str_len, argv[2]))
		return (ft_printf("Failed to transmit string, timed out\n"), 1);
	return (0);
}
