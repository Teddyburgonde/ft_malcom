#include "ft_malcom.h"


volatile sig_atomic_t	g_running = true;

int	main(int argc, char **argv)
{
	if (argc != 5)
	{
		printf("Usage: %s <source_ip> <source_mac> <target_ip> <target_mac>\n",
			argv[0]);
		return (1);
	}
	/* TODO: parsing, signal, socket, wait_arp_request, send_arp_reply */
	return (0);
}