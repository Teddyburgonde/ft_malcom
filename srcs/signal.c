#include "ft_malcom.h"

/*
** Handler appelé quand l'utilisateur fait Ctrl+C (SIGINT).
** Met g_running à false pour sortir proprement de la boucle d'écoute.
*/
void	handler(int sig)
{
	(void)sig;
	g_running = false;
}

/*
** Branche le handler sur le signal SIGINT (Ctrl+C).
*/
void	setup_signal(void)
{
	signal(SIGINT, handler);
}
