// 06-hw-t1 : implementing cp with two arguments from command line

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc != 3) errx(1, "error : usage -> <source> <destination>");

	int src_fd;
	int dest_fd;
	char buff;
	size_t size_buff = 1;

	src_fd = open(argv[1], O_RDONLY);

	if (src_fd == -1)
	{
		err(1, "error : could not open source file!");
	}

	dest_fd = open(argv[2], O_CREAT  | O_TRUNC | O_WRONLY, 0664);

	if (dest_fd == -1)
	{
		const int saved_errno = errno;
		close(src_fd);
		errno = saved_errno;
		err(1, "error : could not open destination file!");
	}

	//reading from src and writing to dest
	while (read(src_fd, &buff, size_buff))
	{
		write(dest_fd, &buff, size_buff);
	}

	if ( close(src_fd) == -1 ) err(1, "Error : could not close source file");
	if ( close(dest_fd) == -1 ) err(1, "Error : could not close destination	file");

	exit(0);
}
