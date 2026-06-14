#include "ft_malcom.h"

int	bind_network_interface(int raw_socket_fd, const char *interface)
{
	struct sockaddr_ll	sll;
	int					index;
	int					bind_result;

	memset(&sll, 0, sizeof(sll));
	index = if_nametoindex(interface);
	if (index == 0)
	{
		perror("if_nametoindex() error");
		exit(2);
	}
	sll.sll_family = AF_PACKET;
	sll.sll_protocol = htons(ETH_P_ARP);
	sll.sll_ifindex = index;
	bind_result = bind(raw_socket_fd, (struct sockaddr *)&sll, sizeof(sll));
	if (bind_result < 0)
	{
		perror("bind() error");
		exit(2);
	}
	return (1);
}


/*

find_network_interface()
Déclarer struct ifaddrs *ifap et un buffer static char name[IF_NAMESIZE]
Appeler getifaddrs(&ifap), vérifier erreur (< 0 → perror + exit)
Boucler sur la liste chaînée (ifa_next)
Pour chaque élément, tester : IFF_UP actif ET IFF_LOOPBACK absent
Si trouvé : copier ifa_name dans name, faire freeifaddrs(ifap), retourner name
Si la boucle se termine sans rien trouver : freeifaddrs(ifap) + message d'erreur + exit



*/