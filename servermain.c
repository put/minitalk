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

#define _XOPEN_SOURCE 700 // lets hope this is not required (or is allowed) at codam
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "server.h"

static volatile servinfo_t info = (servinfo_t){0, 0, {}};

int get_client_index(int pid)
{
	int count;
	
	count = 0;
	while (count < info.totalclients)
	{
		if (info.clients[count].pid == pid)
			return (count);
		count++;
	}
	return (-1);
}

int getbit(int signo)
{
	if (signo == SIGUSR1)
		return (0);
	else
		return (1);
}

void handle_init(volatile recvinfo_t *client, int bit)
{
	printf("Client (%d) sent first lenbit\n", client->pid);
	client->lenbit_count = 1;
	client->str_len = 0;
	client->strbit_count = 0;
	client->state = RECV_LEN;
	client->str_len = (client->str_len << 1) | bit;
	client->curr_byte = 0;
	free(client->res);
	kill(client->pid, SIGUSR1);
}

void handle_recv_len(volatile recvinfo_t *client, int bit)
{
	client->lenbit_count++;
	client->str_len = (client->str_len << 1) | bit;
	if (client->lenbit_count == 32)
	{
		client->state = RECV_STR;
		client->lenbit_count = 0;
		client->res = malloc(client->str_len + 1);
		client->str_len *= 8;
		printf("Full length integer received from (%d): %d\n", client->pid, client->str_len);
	}
	kill(client->pid, SIGUSR1);
}

void handle_recv_str(volatile recvinfo_t *client, int bit)
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
		printf("Received full string from (%d): %s\n", client->pid, client->res);
		free(client->res);
		client->res = NULL;
		client->state = INIT;
	}
	kill(client->pid, SIGUSR1);
}

void handle_new_client(int pid)
{
	info.clients[info.totalclients] = (volatile recvinfo_t){pid, 0, 0, 0, 0, NULL, INIT};
	printf("New client registered on index %d: %d\n", info.totalclients, pid);
	info.totalclients++;
	kill(pid, SIGUSR1);
}

void handle_existing_client_msg(volatile recvinfo_t *client, int bit)
{
	if (client->state == INIT)
		handle_init(client, bit);
	else if (client->state == RECV_LEN)
		handle_recv_len(client, bit);
	else if (client->state == RECV_STR)
		handle_recv_str(client, bit);
}

void handle_sigusr(int signo, siginfo_t *siginf, void *context)
{
	int clientindex;
	recvinfo_t client;

	clientindex = get_client_index(siginf->si_pid);
	if (clientindex < 0)
		handle_new_client(siginf->si_pid);
	else
		handle_existing_client_msg(&(info.clients[clientindex]), getbit(signo));
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

int main(void)
{
	info.ownpid = getpid();
	if (register_handlers())
		return (printf("Failed to register handlers\n"), 1);
	printf("Server initialized with PID: %d\n", info.ownpid);
	while (1)
		pause();
}