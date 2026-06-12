#include "ft_malcom.h"

/*
** Convertit une adresse IP string en format binaire.
*/
int ip_string_to_binary(const char *ip_str, struct in_addr *ip_addr)
{
	if (inet_pton(AF_INET, ip_str, ip_addr) <= 0)
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: (%s).\n", ip_str);
		return -1;
	}
	return 1;
}