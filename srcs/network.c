#include "ft_malcom.h"

/*
** Lie (bind) la raw socket à la carte réseau passée en paramètre,
** pour n'envoyer/recevoir les paquets ARP que sur cette interface.
*/
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
** Trouve la carte réseau active de la machine (hors loopback)
** et retourne son nom (ex: "eth0").
** Le loopback ("lo") est l'interface virtuelle qui permet à la machine
** de communiquer avec elle-même.
*/
char	*find_network_interface(void)
{
	static char		name[IF_NAMESIZE];
	struct ifaddrs	*ifap;
	struct ifaddrs	*it;
	int				check;

	check = getifaddrs(&ifap);
	if (check < 0)
	{
		perror("getifaddrs() error");
		exit(2);
	}
	it = ifap;
	while (it != NULL)
	{
		if ((it->ifa_flags & IFF_UP) && !(it->ifa_flags & IFF_LOOPBACK))
		{
			ft_strcpy(name, it->ifa_name);
			freeifaddrs(ifap);
			return (name);
		}
		it = it->ifa_next;
	}
	freeifaddrs(ifap);
	fprintf(stderr, "ft_malcolm: no network interface found.\n");
	exit(2);
}

