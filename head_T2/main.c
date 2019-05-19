
#include <stdio.h>			//printf()

#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdlib.h>

//program implementing head

int main(int argc, char* argv[])
{	
	if (argc != 2)
		errx(9, "1 argument needed!\n");

	int fd;
	int rowcount = 0;
	char buff;

	fd = open(argv[1], O_RDONLY);

	if (fd == -1)
		errx(10, "could not open file!\n");

	while (read(fd, &buff, sizeof(buff)))
	{
		if (buff == '\n')
			rowcount++;

		write(1, &buff, sizeof(buff));

		if (rowcount == 10)
			break;
	}

	close(fd);

	exit(0);
}
