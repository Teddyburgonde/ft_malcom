#include "ft_malcom.h"

/*
** Convertit une adresse IP string en format binaire.
*/
int	ip_string_to_binary(const char *ip_str, struct in_addr *ip_addr)
{
	if (inet_pton(AF_INET, ip_str, ip_addr) <= 0)
	{
		fprintf(stderr,
			"ft_malcolm: unknown host or invalid IP address: (%s)\n", ip_str);
		return (-1);
	}
	return (1);
}

/*
** Convertit un caractère hexadécimal en sa valeur (0 à 15).
** Retourne -1 si le caractère n'est pas un hexa valide.
*/
static int	hex_to_int(char c)
{
	int	value;

	if (c >= '0' && c <= '9')
		value = c - '0';
	else if (c >= 'a' && c <= 'f')
		value = c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		value = c - 'A' + 10;
	else
		value = -1;
	return (value);
}

/*
** Convertit une adresse MAC string "xx:xx:xx:xx:xx:xx" en 6 octets binaires.
** Valide le format (6 octets hexa séparés par ':') et remplit mac[6].
** Un nibble = un demi-octet (4 bits) : top_nibble = chiffre de gauche,
** low_nibble = chiffre de droite ; les deux fusionnés forment 1 octet.
** Retourne 1 si l'adresse est valide, -1 sinon.
*/
int	mac_string_to_binary(const char *mac_str, uint8_t *mac)
{
	int	i;
	int	top_nibble;
	int	low_nibble;

	i = 0;
	while (i < 6)
	{
		top_nibble = hex_to_int(mac_str[i * 3]);
		low_nibble = hex_to_int(mac_str[i * 3 + 1]);
		if (top_nibble < 0 || low_nibble < 0)
			break ;
		mac[i] = top_nibble * 16 + low_nibble; // fusion des 2 valeurs (gauche + droite) en 1 octet
		if (i < 5 && mac_str[i * 3 + 2] != ':')
			break ;
		if (i == 5 && mac_str[i * 3 + 2] != '\0')
			break ;
		i++;
	}
	if (i != 6)
	{
		fprintf(stderr, "ft_malcolm: invalid mac address: (%s)\n", mac_str);
		return (-1);
	}
	return (1);
}

/*
** Parse et valide les 4 arguments dans l'ordre imposé par le sujet :
**   argv[1] source_ip   argv[2] source_mac
**   argv[3] target_ip   argv[4] target_mac
** Remplit la structure cfg. Retourne 1 si OK, -1 sinon.
*/
int	parse_arguments(int argc, char **argv, t_config *cfg)
{
	if (argc != 5)
	{
		fprintf(stderr,
			"Usage: %s <source_ip> <source_mac> <target_ip> <target_mac>\n",
			argv[0]);
		return (-1);
	}
	if (ip_string_to_binary(argv[1], &cfg->source_ip) < 0)
		return (-1);
	if (mac_string_to_binary(argv[2], cfg->source_mac) < 0)
		return (-1);
	if (ip_string_to_binary(argv[3], &cfg->target_ip) < 0)
		return (-1);
	if (mac_string_to_binary(argv[4], cfg->target_mac) < 0)
		return (-1);
	return (1);
}
