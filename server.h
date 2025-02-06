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

#ifndef MINITALK_SERVER_H
# define MINITALK_SERVER_H
# ifndef MAX_CLIENTS
#  define MAX_CLIENTS 1024
# endif

typedef enum clientstate_t {
	INIT,
	RECV_LEN,
	RECV_STR
} clientstate_t;

typedef struct recvinfo_t {
	int pid;
	int lenbit_count;
	int strbit_count;
	int str_len;
    char curr_byte;
    char *res;
	clientstate_t state;
} recvinfo_t;

typedef struct servinfo_t {
	int ownpid;
	int totalclients;
	volatile recvinfo_t clients[MAX_CLIENTS];
} servinfo_t;
#endif