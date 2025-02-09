/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   server.h										   :+:	:+:			*/
/*													 +:+					*/
/*   By: mika <mika@student.codam.nl>				 +#+					 */
/*												   +#+					  */
/*   Created: 2025/02/06 15:14:08 by mika		  #+#	#+#				 */
/*   Updated: 2025/02/06 16:50:52 by mika		  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H
# define MAX_CLIENTS 2048
# include <signal.h>

typedef enum t_clientstate
{
	INIT,
	RECV_LEN,
	RECV_STR,
	DONE,
}	t_clientstate;

typedef struct t_client
{
	volatile sig_atomic_t	pid;
	sig_atomic_t			lenbit_count;
	sig_atomic_t			strbit_count;
	sig_atomic_t			str_len;
	sig_atomic_t			curr_byte;
	char					*res;
	volatile sig_atomic_t	state;
}	t_client;

typedef struct t_server
{
	int						ownpid;
	volatile t_client		clients[MAX_CLIENTS];
}	t_server;

int					getbit(int signo);
void				handle_init(volatile t_client *client);
#endif
