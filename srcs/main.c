#include "ft_malcom.h"


volatile sig_atomic_t	g_running = true;

int	main(int argc, char **argv)
{
	t_config	cfg;
	int			raw_socket_fd;
	char		*interface;
	int			found;

	if (!is_root())
	{
		fprintf(stderr, "ft_malcolm: you must be root to run this program.\n");
		return (1);
	}
	if (parse_arguments(argc, argv, &cfg) < 0)
		return (1);
	setup_signal();
	raw_socket_fd = create_raw_socket();
	interface = find_network_interface();
	bind_network_interface(raw_socket_fd, interface);
	found = wait_arp_request(raw_socket_fd, &cfg);
	if (found)
		send_arp_reply(raw_socket_fd, &cfg, interface);
	close(raw_socket_fd);
	return (0);
}