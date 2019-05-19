
#include <stdio.h>			//printf()

#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdlib.h>

//program implementing cat with variable cmd arguments

int main(int argc, char* argv[])
{	
	if (argc < 2)
		errx(9, "at least 1 argument needed!\n");

	int fd;
	char buff;

	for (int i = 0; i < argc - 1; i++)
	{
		fd = open(argv[i + 1], O_RDONLY);

		if (fd == -1)
		{
			errx(11, "could not open file!");
		}

		while (read(fd, &buff, 1))
		{
			write(1, &buff, 1);
		}

		close(fd);
	}

//	printf("\n");

	exit(0);
}
