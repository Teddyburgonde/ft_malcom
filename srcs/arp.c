#include "ft_malcom.h"

/*
** Attend en boucle arrivé d'une requête ARP qui demande l'IP de la machine qu'on usurpe.
*/
int	wait_arp_request(int raw_socket_fd, t_config *cfg)
{
	unsigned char	buffer[42];
	t_arp_packet	*arp;
	int				check;

	arp = (t_arp_packet *)(buffer + sizeof(t_ethernet_frame));
	while (g_running)
	{
		check = recvfrom(raw_socket_fd, buffer, sizeof(buffer), 0, NULL, NULL);
		if (check < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;
			perror("recvfrom() error");
			exit(2);
		}
		if (ntohs(arp->operation) == ARP_REQUEST
			&& arp->target_ip == cfg->source_ip.s_addr)
			return (1);
	}
	return (0);
}

// A FAIRE
int	send_arp_reply(int raw_socket_fd, t_config *cfg)
{
	(void)raw_socket_fd;
	(void)cfg;
	return (0);
}