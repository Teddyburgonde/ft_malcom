#include "ft_malcom.h"

/*
Permet de savoir si utilisateur est root.
*/
int	is_root(void)
{
	if (getuid() != 0)
	{
		return false;
	}
	return true;
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
		perror("socket() error");
		exit(2);
	}
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	if (setsockopt(raw_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout,
			sizeof(timeout)) < 0)
	{
		perror("setsockopt() error");
		close(raw_socket);
		exit(2);
	}
	return (raw_socket);
}