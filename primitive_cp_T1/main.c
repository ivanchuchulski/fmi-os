

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <stdio.h>
#include <stdlib.h>

//program implementing cp

int main()
{
	int fd_src;
	int fd_dest;
	char buff;

	fd_src = open("foo.txt", O_RDONLY);

	if (fd_src == -1)
	{
		errx(10, "Could not open source file!\n");
	}

	fd_dest = open("bar.txt", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG, 0666);

	if (fd_dest == -1)
	{
		errx(11, "Could not open destination file!\n");
		close(fd_src);
	}

	while (read(fd_src, &buff, sizeof(buff)))
	{
		write(fd_dest, &buff, sizeof(buff));
	}

	close(fd_src);
	close(fd_dest);

	exit(0);
}
