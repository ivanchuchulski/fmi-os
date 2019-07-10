// 06-hw-task1 : implementing cp with two arguments, arg names are hardcoded

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int src_fd;
	int dest_fd;
	char buff;
	size_t size_buff = 1;

	src_fd = open("foo.txt", O_RDONLY);

	if (src_fd == -1)
	{
		err(1, "error : open foo.txt");
	}

	dest_fd = open("bar.txt", O_CREAT | O_TRUNC | O_WRONLY, 0664);

	if (dest_fd == -1)
	{
		const int saved_errno = errno;
		close(src_fd);
		errno = saved_errno;
		err(1, "error : open bar.txt");
	}

	while (read(src_fd, &buff, size_buff))
	{
		write(dest_fd, &buff, size_buff);
	}

	if (close(src_fd) == -1)
	{
		 err(1, "error : close foo.txt");	
	}

	if (close(dest_fd) == -1) 
	{
		err(1, "error : close bar.txt");
	}
	
	exit(0);
}
