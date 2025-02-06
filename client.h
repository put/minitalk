/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/06 20:01:31 by mika          #+#    #+#                 */
/*   Updated: 2025/02/06 21:18:19 by mika          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H
typedef enum clienstate_t {
	IDLE,
	WAIT_INIT_CONF,
	WAIT_INT_CONF,
	WAIT_STR_CONF
} clienstate_t;

typedef struct clientinfo_t {
	int serv_pid;
	volatile clienstate_t state;
} clientinfo_t;
#endif