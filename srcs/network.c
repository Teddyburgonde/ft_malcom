#include "ft_malcom.h"

/*
** Lie (bind) la raw socket à la carte réseau passée en paramètre,
** pour n'envoyer/recevoir les paquets ARP que sur cette interface.
*/
int	bind_network_interface(int raw_socket_fd, const char *interface)
{
	int	result;

	result = setsockopt(raw_socket_fd, SOL_SOCKET, SO_BINDTODEVICE,
			interface, ft_strlen(interface));
	if (result < 0)
	{
		printf("setsockopt() error\n");
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
		printf("getifaddrs() error\n");
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

