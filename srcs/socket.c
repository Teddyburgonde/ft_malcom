#include "ft_malcom.h"

/*
Permet de savoir si utilisateur est root.
*/
int	is_root(void)
{
	return getuid() == 0;
}

/*
** Crée une raw socket pour recevoir et envoyer des paquets ARP.
** Active un timeout de 5 secondes pour éviter le blocage infini.
*/
int create_raw_socket(void)
{
	int raw_socket;
	struct timeval timeout;

	raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (raw_socket < 0)
	{
		printf("socket() error\n");
		exit(2);
	}
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	if (setsockopt(raw_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout,
			sizeof(timeout)) < 0)
	{
		printf("setsockopt() error\n");
		close(raw_socket);
		exit(2);
	}
	return (raw_socket);
}