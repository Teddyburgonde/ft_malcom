#include "ft_malcom.h"


volatile sig_atomic_t	g_running = true;

int	main(int argc, char **argv)
{
	t_config	cfg;

	if (!is_root())
	{
		fprintf(stderr, "ft_malcolm: you must be root to run this program.\n");
		return (1);
	}
	if (parse_arguments(argc, argv, &cfg) < 0)
		return (1);
	/* TODO: signal, socket, wait_arp_request, send_arp_reply */
	return (0);
}