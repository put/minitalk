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
# include <signal.h>

typedef enum t_clientstate
{
	INIT,
	RECV_LEN,
	RECV_STR
}	t_clientstate;

typedef struct t_client
{
	volatile int			pid;
	int						lenbit_count;
	int						strbit_count;
	int						str_len;
	char					curr_byte;
	char					*res;
	volatile t_clientstate	state;
}	t_client;

typedef struct t_server
{
	int						ownpid;
	volatile sig_atomic_t	busy;
	volatile t_client		client;
}	t_server;

int					getbit(int signo);
volatile t_server	*servinfo(void);
void				handle_init(volatile t_client *client);
#endif
