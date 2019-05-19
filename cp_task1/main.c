
#include <stdio.h>			//printf()

#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdlib.h>

//program implementing cp with two arguments

int main(int argc, char* argv[])
{
	int fd_src;
	int fd_dest;
	char buff;

	if (argc != 3)
	{
		errx(5, "2 arguments needed!");
	}


	fd_src = open(argv[1], O_RDONLY);

	if (fd_src == -1)
	{
		errx(10, "Could not open source file!\n");
	}

	fd_dest = open(argv[2], O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if (fd_dest == -1)
	{
		errx(11, "Could not open destination file!\n");
		close(fd_src);
	}

	//reading from src and writing to dest
	while (read(fd_src, &buff, sizeof(buff)))
	{
		write(fd_dest, &buff, sizeof(buff));
	}

	close(fd_src);
	close(fd_dest);

	exit(0);
}
