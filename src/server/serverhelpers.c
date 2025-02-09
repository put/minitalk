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

void	handle_init(volatile t_client *client)
{
	client->lenbit_count = 0;
	client->str_len = 0;
	client->strbit_count = 0;
	client->state = RECV_LEN;
	client->str_len = 0;
	client->curr_byte = 0;
	free(client->res);
	ft_printf("Init received from %d\n", client->pid);
	kill(client->pid, SIGUSR1);
}
