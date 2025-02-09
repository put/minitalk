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
#include "server.h"
#include "../shared.h"

static t_server g_server;

static volatile t_client *get_client(int pid)
{
	int count;

	count = 0;
	while (count < MAX_CLIENTS)
	{
		if (g_server.clients[count].pid == pid)
			return (&(g_server.clients[count]));
		count++;
	}
	count = 0;
	while (count < MAX_CLIENTS)
	{
		if (g_server.clients[count].pid == 0
			|| g_server.clients[count].state == DONE)
		{
			g_server.clients[count].pid = pid;
			g_server.clients[count].state = INIT;
			ft_printf("New client: %d\n", pid);
			return (&(g_server.clients[count]));
		}
	}
	return (NULL);
}

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
	ft_printf("Sending SIGUSR1 to client %d\n", client->pid);
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
		client->state = DONE;
		free(client->res);
		client->res = NULL;
	}
	ft_printf("Sending SIGUSR1 to client %d\n", client->pid);
	kill(client->pid, SIGUSR1);
}

void	handle_client_msg(volatile t_client *client, int bit)
{
	ft_printf("Client msg from %d state %d\n", client->pid, client->state);
	if (client->state == INIT)
		handle_init(client);
	else if (client->state == RECV_LEN)
		handle_recv_len(client, bit);
	else if (client->state == RECV_STR)
		handle_recv_str(client, bit);
}

void	handle_sigusr(int signo, siginfo_t *siginf, void *context)
{
	volatile t_client *client;

	(void)context;
	client = get_client(siginf->si_pid);
	if (!client)
	{
		ft_printf("Unhandled message from %d\n", siginf->si_pid);
		return ;
	}
	handle_client_msg(client, getbit(signo));
}

int	main(void)
{
	g_server.ownpid = getpid();
	if (register_handlers(&handle_sigusr))
		return (ft_printf("Failed to register handlers\n"), 1);
	ft_printf("Server initialized with PID: %d\n", g_server.ownpid);
	while (1)
		pause();
}
