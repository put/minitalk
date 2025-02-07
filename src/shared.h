/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shared.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/07 01:53:50 by mika          #+#    #+#                 */
/*   Updated: 2025/02/07 03:17:44 by mika          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
# define SHARED_H
# include <signal.h>
# include <unistd.h>
# include "../libft/libft.h"

int	register_handlers(void *handler_func);
#endif