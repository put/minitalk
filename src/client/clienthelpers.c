/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   clienthelpers.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/07 02:36:08 by mika          #+#    #+#                 */
/*   Updated: 2025/02/07 02:39:37 by mika          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

volatile t_clientinfo	*clientinfo(void)
{
	static volatile t_clientinfo	info;
	static int						first_call = 1;

	if (first_call)
	{
		info.serv_pid = 0;
		info.state = IDLE;
		first_call = 0;
	}
	return (&info);
}
