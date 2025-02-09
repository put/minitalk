/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/06 20:01:31 by mika          #+#    #+#                 */
/*   Updated: 2025/02/08 13:13:12 by mika          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H
# include <signal.h>

typedef enum t_clientstate
{
	IDLE,
	INIT_DONE,
	SENDING_STR,
	SENDING_INT,
	WAIT_SEND,
	TIMEOUT,
	FAIL,
}	t_clientstate;

typedef struct t_clientinfo
{
	int						serv_pid;
	volatile sig_atomic_t	state;
}	t_clientinfo;

volatile t_clientinfo	*clientinfo(void);
int						bit_to_sig(int bit);
#endif