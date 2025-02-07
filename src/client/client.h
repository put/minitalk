/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/06 20:01:31 by mika          #+#    #+#                 */
/*   Updated: 2025/02/07 02:37:55 by mika          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

typedef enum t_clientstate
{
	IDLE,
	WAIT_INIT_CONF,
	WAIT_INT_CONF,
	WAIT_STR_CONF,
	FAIL,
}	t_clientstate;

typedef struct t_clientinfo
{
	int						serv_pid;
	volatile t_clientstate	state;
}	t_clientinfo;

volatile t_clientinfo	*clientinfo(void);
#endif