/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   servermain.c									   :+:	:+:			*/
/*													 +:+					*/
/*   By: mika <mika@student.codam.nl>				 +#+					 */
/*												   +#+					  */
/*   Created: 2025/02/06 15:06:32 by mika		  #+#	#+#				 */
/*   Updated: 2025/02/06 18:23:43 by mika		  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <unistd.h>
#include "server.h"
#include "../shared.h"

void	handle_recv_len(volatile t_client *client, int bit)
{
	client->lenbit_count++;
	client->str_len = (client->str_len << 1) | bit;
	if (client->lenbit_count == 32)
	{
		client->state = RECV_STR;
		client->lenbit_count = 0;
		client->res = malloc(client->str_len + 1);
		client->str_len *= 8;
	}
	kill(client->pid, SIGUSR1);
}

void	handle_recv_str(volatile t_client *client, int bit)
{
	client->curr_byte = (client->curr_byte << 1) | bit;
	client->strbit_count++;
	if (client->strbit_count % 8 == 0 && client->strbit_count > 0)
	{
		client->res[(client->strbit_count / 8) - 1] = client->curr_byte;
		client->curr_byte = 0;
	}
	if (client->strbit_count == client->str_len)
	{
		client->res[client->str_len / 8] = 0;
		ft_printf("(PID %d) -> %s\n", client->pid, client->res);
		servinfo()->busy = 0;
		client->state = INIT;
		free(client->res);
		client->res = NULL;
	}
	kill(client->pid, SIGUSR1);
}

void	handle_client_msg(volatile t_client *client, int bit)
{
	if (client->state == INIT)
		handle_init(client);
	else if (client->state == RECV_LEN)
		handle_recv_len(client, bit);
	else if (client->state == RECV_STR)
		handle_recv_str(client, bit);
}

void	handle_sigusr(int signo, siginfo_t *siginf, void *context)
{
	(void)context;
	if (siginf->si_pid != servinfo()->client.pid)
	{
		if (!servinfo()->busy)
		{
			servinfo()->client.pid = siginf->si_pid;
			servinfo()->client.state = INIT;
			servinfo()->busy = 1;
			handle_client_msg(&(servinfo()->client), getbit(signo));
		}
	}
	else if (siginf->si_pid == servinfo()->client.pid)
		handle_client_msg(&(servinfo()->client), getbit(signo));
}

int	main(void)
{
	servinfo()->ownpid = getpid();
	if (register_handlers(&handle_sigusr))
		return (ft_printf("Failed to register handlers\n"), 1);
	ft_printf("Server initialized with PID: %d\n", servinfo()->ownpid);
	while (1)
		pause();
}
