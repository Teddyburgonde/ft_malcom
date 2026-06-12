#include "ft_malcom.h"

/*
Permet de savoir si utilisateur est root.
*/
int	is_root(void)
{
	if (getuid() != 0)
	{
		return false;
	}
	return true;
}