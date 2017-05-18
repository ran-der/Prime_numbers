#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int			open_error(char *file)
{
	printf("Error\nCouldn't open/read '%s' or file is empty.", file);
	return (0);
}

int			usage(void)
{
	printf("Error\nUsage:\n    base_chains [-l <length>] <number_file>\n");
	return (0);
}

void		ft_putchar(char c)
{
	write(1, &c, 1);
}

void		ft_putstr(char *str)
{
	write(1, str, strlen(str));
}

void		ft_putnbr(int nb)
{
	if (nb < 0)
	{
		ft_putchar('-');
		nb = -nb;
	}
	if (nb > 9)
		ft_putnbr(nb / 10);
	ft_putchar('0' + nb % 10);
}

char		*ft_strjoin(char *s1, char *s2)
{
	char	*ret;
	int		l1;
	int		l2;
	
	l1 = (s1 == NULL ? 0 : strlen(s1));
	l2 = (s2 == NULL ? 0 : strlen(s2));
	if (!(l1 + l2) || !(ret = (char*)malloc(sizeof(char) * (l1 + l2 + 1))))
		return (NULL);
	if (s1 != NULL)
		strcpy(ret, s1);
	if (s2 != NULL)
		strcpy(ret + l1, s2);
	return (ret);
}

int			reduce(int nb, int base)
{
	int		sum;
	
	if (nb < 0)
		nb = -nb;
	if (nb < base)
		return (nb);
	sum = 0;
	while (nb > 0)
	{
		sum += nb % base;
		nb /= base;
	}
	return (reduce(sum, base));
}

void		print_chain(int nb, int len)
{
	int		base;

	printf("(%-10d) ", nb);
	len += 2;
	base = 1;
	while (++base < len)
	{
		printf("%d", base);
		if (reduce(nb, base + 1) == nb % base)
			printf("-");
		else
			printf("\x1B[31m|\x1B[0m");
	}
	printf("%d", base);
	printf("\n");
}

char		*read_file(char	*file)
{
	char	buf[1024];
	char	*ret;
	char	*old;
	int		fd;
	int		rd;

	if ((fd = open(file, O_RDONLY)) < 3)
		return (NULL);
	ret = NULL;
	while ((rd = read(fd, buf, 1023)))
	{
		buf[rd] = '\0';
		old = ret;
		ret = ft_strjoin(old, buf);
		if (old != NULL)
			free(old);
	}
	return (ret);
}

int			main(int argc, char **argv)
{
	char	*file;
	int		i;
	int		len;

	if (argc > 4 || argc < 2)
		return (usage());
	len = 30;
	if (!strcmp(argv[1], "-l"))
	{
		if (argc != 4)
			return (usage());
		len = atoi(argv[2]);
		if (!(file = read_file(argv[3])))
			return (open_error(argv[3]));
	}
	else if (!(file = read_file(argv[1])))
		return (open_error(argv[1]));
	i = -1;
	while (file[++i])
	{
		if (file[i] <= '9' && file[i] >= '0')
		{
			print_chain(atoi(file + i), len);
			while (file[i + 1] != '\0' && file[i] <= '9' && file[i] >= '0')
				i++;
		}
	}
	return (0); 
}

