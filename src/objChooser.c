#include "objChooser.h"

static char	*ft_itoa(int n)
{
	char	*res;
	int		ln;
	int		t;

	ln = (n <= 0) ? 1 : 0;
	t = n;
	while (t != 0)
	{
		t /= 10;
		ln++;
	}
	if (!(res = malloc(ln + 1)))
		exit(EXIT_FAILURE);
	if (n == 0)
		res[0] = '0';
	res[ln] = 0;
	if (n < 0)
		res[0] = '-';
	while (n != 0)
	{
		ln--;
		res[ln] = '0' + (n % 10 < 0 ? -(n % 10) : n % 10);
		n /= 10;
	}
	return (res);
}

static int	rDir(char *buff, int num)
{
	DIR				*dir;
	struct dirent	*ent;
	int				tmp;
	char			buf[50];

	tmp = 0;
	if (!buff)
		printf("Avaliable objects:\n\n");
	if ((dir = opendir("res/objects/")) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			if (strlen(ent->d_name) < 50 && !strcmp(&(ent->d_name[strlen(ent->d_name) - 4]), ".obj"))
			{
				strcpy(buf, ent->d_name);
				buf[strlen(ent->d_name) - 4] = 0;
				if (!buff)
					printf("%i. %s\n", tmp, buf);
				else if (tmp == num || !strcmp(buf, buff))
				{
					strcpy(buff, buf);
					closedir(dir);
					return 1;
				}
				++tmp;
			}
		}
		closedir(dir);
	}
	else
	{
		printf("Could not find folder with objects!\n");
		exit(EXIT_FAILURE);
	}
	if (tmp == 0)
	{
		printf("Could not find objects!\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}

void	objChooser(char *buff)
{
	int		i = 0;
	rDir(0, -1);
	while (!i)
	{
		write(1, "\nEnter object name or number: ", 30);
		int r = read(0, buff, 50);
		if (r == 50 || r <= 0)
		{
			printf("\nWrong name or number!\n");
			continue ;
		}
		if (buff[r - 1] == '\n')
			--r;
		buff[r] = 0;
		i = atoi(buff);
		char	*a = ft_itoa(i);
		if (i >= 0 && !strcmp(buff, a))
			i = rDir(buff, i);
		else
			i = rDir(buff, -1);
		free(a);
		if (!i)
			printf("Wrong name or number!\n");
	}
}
