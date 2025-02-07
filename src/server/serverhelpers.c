/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   serverhelpers.c									:+:	:+:			*/
/*													 +:+					*/
/*   By: mika <mika@student.codam.nl>				 +#+					 */
/*												   +#+					  */
/*   Created: 2025/02/07 02:11:33 by mika		  #+#	#+#				 */
/*   Updated: 2025/02/07 02:32:51 by mika		  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#include "../shared.h"
#include "server.h"

int	getbit(int signo)
{
	if (signo == SIGUSR1)
		return (0);
	else
		return (1);
}

volatile t_server	*servinfo(void)
{
	static volatile t_server	info;
	static int					first_call = 1;

	if (first_call)
	{
		info.ownpid = 0;
		info.busy = 0;
		info.client.pid = 0;
		info.client.lenbit_count = 0;
		info.client.strbit_count = 0;
		info.client.str_len = 0;
		info.client.curr_byte = 0;
		info.client.res = NULL;
		info.client.state = INIT;
		first_call = 0;
	}
	return (&info);
}

void	handle_init(volatile t_client *client)
{
	client->lenbit_count = 0;
	client->str_len = 0;
	client->strbit_count = 0;
	client->state = RECV_LEN;
	client->str_len = 0;
	client->curr_byte = 0;
	free(client->res);
	kill(client->pid, SIGUSR1);
}
