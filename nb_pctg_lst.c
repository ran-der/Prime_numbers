#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int		err_malloc(void)
{
	write(2, "\nSomething went wrong with pointer allocation or liberation.\n", 61);
	return (-1);
}

int		err_open(char *file_name)
{
	write(2, "\nSomething went wrong when opening file \"", 41);
	write(2, file_name, strlen(file_name));
	write(2, "\"\n", 2);
	return (-1);
}
	
int		err_usage(void)
{
	write(2, "\nWrong number of arguments.\n", 28);
	write(2, "usage:\n\n  ./pctg_list [(char*)input file] [(char*)base]\n", 56);
	return (-1);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr_base(int nbr, char *base, int len)
{	
	if (nbr < 0)
	{
		ft_putchar('-');
		nbr = -nbr;
	}
	if (nbr >= len)
	{
		ft_putnbr_base(nbr / len, base, len);
		ft_putnbr_base(nbr % len, base, len);
	}
	else
		ft_putchar(base[nbr]);
}

char	*ft_join_char(char *str, char c)
{
	char	*new;
	int		i;
	
	if ((new = (str != NULL ? (char*)malloc(sizeof(char) * strlen(str) + 2) :\
			(char*)malloc(sizeof(char) * 2))) == NULL)
		return (NULL);
	i = 0;
	while (str != NULL && str[i] != '\0')
	{
		new[i] = str[i];
		i++;
	}
	new[i] = c;
	new[++i] = '\0';
	if (str != NULL)
		free(str);
	return (new);
}

float		*init_new_tab(int size, float val)
{
	float	*new;
	int		i;
	
	if ((new = (float*)malloc(sizeof(float) * size)) == NULL)
		return (NULL);
	i = -1;
	while (++i < size)
		new[i] = val;
	return (new);
}

int		get_rdigit_val(char rdigit, char *base)
{
	int		i;
	
	i = 0;
	while (base[i] && base[i] != rdigit)
		i++;
	return (base[i] ? i : -1);
}

char	get_rdigit(int nb, char *base, int bsize)
{
	int		res;
	
	if (nb < bsize)
		return (base[nb]);
	res = 0;
	while (nb > 0)
	{
		res += nb % bsize;
		nb = nb / bsize;
	}
	return (get_rdigit(res, base, bsize));
}

void	print_pctgs(char *base, float *qtts, int bsize)
{
	float	total;
	int		i;
	
	i = -1;
	total = 0;
	while (++i < bsize)
		total += qtts[i];
	i = -1;
	while (++i < bsize)
		printf("%c(%d) %f%%\n", base[i], (int)qtts[i], qtts[i] * 100. / total);
}
	
void	print_list(char *list, int bsize)
{
	int		i;
	int		start;
	
	i = -1;
	start = 1;
	while (list[++i])
	{
		if (start)
		{
			write(1, list + i, 1);
			start = 0;
		}
		else
		{
			write(1, "  ", 2);
			write(1, list + i, 1);
		}
		if (!((i + 1) % (bsize - 1)))
		{
			ft_putchar('\n');
			start = 1;
		}
	}
}

int		main(int argc, char **argv)
{
	char	*list = NULL;
	float	*qtts;
	int		fd;
	char	buff[1];
	int		nb;
	int		bsize;
	int		counter = 0;
	char	rdigit;
	char	*nbr = NULL;
	

	if (argc != 3)
		return(err_usage());
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		return (err_open(argv[1]));
	bsize = strlen(argv[2]);
	if ((qtts = init_new_tab(bsize, 0.)) == NULL)
		return(err_malloc());
	while (read(fd, buff, 1) != 0)
	{
		if (*buff <= '9' && *buff >= '0')
		{
			if ((nbr = ft_join_char(nbr, *buff)) == NULL)
				return (err_malloc());
		}
		else
		{
			write(1, "  ", 2);
			counter++;
			nb = atoi(nbr);
			nb = (nb < 0 ? -nb : nb);
			rdigit = get_rdigit(nb, argv[2], bsize);
			ft_putnbr_base(nb, argv[2], bsize);
			write(1, "->", 2);
			ft_putchar(rdigit);
			if (!(counter % (bsize)))
				ft_putchar('\n');
			qtts[get_rdigit_val(rdigit, argv[2])] += 1.;
			if ((list = ft_join_char(list, rdigit)) == NULL)
				return (err_malloc());
			free(nbr);
			nbr = NULL;
		}
	}
	close(fd);
	write(1, "\n", 1);
	print_list(list, bsize);
	write(1, "\n\n", 2);
	print_pctgs(argv[2], qtts, bsize);
	free(list);
	free(qtts);
	return (0);
}
