#ifndef FT_MALCOM_H
# define FT_MALCOM_H

/* ************************************************************************** */
/*                                INCLUDES                                    */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <signal.h>
# include <errno.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <net/if.h>
# include <ifaddrs.h>

/* Spécifique Linux (raw socket niveau 2)                                    */
# include <netpacket/packet.h>
# include <net/ethernet.h>
# include <linux/if_ether.h>

/* ************************************************************************** */
/*                                 DEFINES                                    */
/* ************************************************************************** */

# define ARP_REQUEST	1
# define ARP_REPLY		2

# define MAC_LEN		6
# define IPV4_LEN		4

/* ************************************************************************** */
/*                                STRUCTURES                                  */
/* ************************************************************************** */

/*
** Structure d'un paquet ARP
*/
typedef struct s_arp_packet
{
	uint16_t	hw_type;		/* Type de matériel (Ethernet = 1) */
	uint16_t	proto_type;		/* Type de protocole (IPv4 = 0x0800) */
	uint8_t		hw_len;			/* Longueur adresse matérielle (6 pour MAC) */
	uint8_t		proto_len;		/* Longueur adresse protocole (4 pour IPv4) */
	uint16_t	operation;		/* Opération (1 = Requête, 2 = Réponse) */
	uint8_t		sender_mac[MAC_LEN];	/* Adresse MAC de l'émetteur */
	uint32_t	sender_ip;		/* Adresse IP de l'émetteur */
	uint8_t		target_mac[MAC_LEN];	/* Adresse MAC de la cible */
	uint32_t	target_ip;		/* Adresse IP de la cible */
}	t_arp_packet;

/*
** Structure d'une Ethernet Frame (header Ethernet)
*/
typedef struct s_ethernet_frame
{
	uint8_t		dest_mac[MAC_LEN];	/* Adresse MAC de destination */
	uint8_t		src_mac[MAC_LEN];	/* Adresse MAC source */
	uint16_t	ethertype;		/* Type Ethernet (0x0806 pour ARP) */
}	t_ethernet_frame;

/*
** Structure qui regroupe les 4 arguments parsés du programme
*/
typedef struct s_config
{
	struct in_addr	source_ip;		/* IP à usurper (arg 1) */
	uint8_t			source_mac[MAC_LEN];	/* MAC falsifiée (arg 2) */
	struct in_addr	target_ip;		/* IP de la cible (arg 3) */
	uint8_t			target_mac[MAC_LEN];	/* MAC de la cible (arg 4) */
}	t_config;

/* ************************************************************************** */
/*                            VARIABLE GLOBALE                                */
/* ************************************************************************** */

/* Seule variable globale autorisée : flag de la boucle d'écoute (Ctrl+C).   */
/* Définie dans main.c, déclarée extern ici pour être visible partout.       */
extern volatile sig_atomic_t	g_running;

/* ************************************************************************** */
/*                               PROTOTYPES                                   */
/* ************************************************************************** */

/* parsing.c */
int		ip_string_to_binary(const char *ip_str, struct in_addr *ip_addr);
int		mac_string_to_binary(const char *mac_str, uint8_t *mac);
int		parse_arguments(int argc, char **argv, t_config *cfg);

/* signal.c */
void	handler(int sig);
void	setup_signal(void);

/* socket.c */
int		create_raw_socket(void);
int		is_root(void);

/* network.c */
char	*find_network_interface(void);
int		bind_network_interface(int sock, const char *interface);

/* arp.c */
int		wait_arp_request(int sock, t_config *cfg);
int		send_arp_reply(int sock, t_config *cfg);

#endif
