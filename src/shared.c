/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shared.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/07 01:53:00 by mika          #+#    #+#                 */
/*   Updated: 2025/02/07 02:02:05 by mika          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 700
#include "shared.h"

int	register_handlers(void *handler_func)
{
	int					returnvals;
	struct sigaction	sa;

	sa.sa_sigaction = handler_func;
	sa.sa_flags = SA_SIGINFO;
	returnvals = 0;
	returnvals += sigemptyset(&sa.sa_mask);
	returnvals += sigaddset(&sa.sa_mask, SIGUSR1);
	returnvals += sigaddset(&sa.sa_mask, SIGUSR2);
	returnvals += sigaction(SIGUSR1, &sa, NULL);
	returnvals += sigaction(SIGUSR2, &sa, NULL);
	return (returnvals);
}
