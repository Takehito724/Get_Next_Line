#include <fcntl.h>
#include "get_next_line.h"
#include <stdio.h>

int		main(void)
{
	int		fd;
	int		res;
	int		n;
	char	*line;

	fd = open("heike.txt", O_RDONLY);
	n = 1;
	while (res != 0 && res != -1)
	{
		res = get_next_line(fd, &line);
		printf("%d行目：%s\n戻り値：%d\n",n,line,res);
		n++;
	}
	return (0);
}