// 06-t2 : implementing head without options

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{	
	if (argc != 2)
	{
		errx(1, "error : 1 argument needed");
	}

	int fd;
	char buff;
	size_t size_buff = 1;
	int rowcount = 0;

	fd = open(argv[1], O_RDONLY);

	if (fd == -1)
	{
		err(1, "error : open %s", argv[1]);
	}

	while (read(fd, &buff, size_buff))
	{
		if (buff == '\n')
			rowcount++;

		write(1, &buff, size_buff);

		if (rowcount == 10)
			break;
	}

	if (close(fd) == -1) 
	{
		err(1, "error : close %s", argv[1]);
	}

	exit(0);
}
