#include <stdio.h>
#include <stdlib.h>

int			main(int argc, char **argv)
{
	int		i;
	int		lim;

	if (argc != 2)
		return (0);
	lim = atoi(argv[1]);
	i = 0;
	while (++i <= lim)
		printf("%d ", i);
	return (0);
}
