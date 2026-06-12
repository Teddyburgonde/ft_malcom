#include "ft_malcom.h"

void handler(int sig)
{
	(void)sig;

	g_running = false;
}
