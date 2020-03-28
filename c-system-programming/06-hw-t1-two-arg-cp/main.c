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
	if (argc != 3)
	{
		errx(1, "error : incorrect usage, \n\tcp <source> <destination>");
	}

	int src_fd;
	int dest_fd;

	const size_t size_buff = 256;
	char buff[size_buff];

	src_fd = open(argv[1], O_RDONLY);

	if (src_fd == -1)
	{
		err(1, "error : open source file");
	}

	dest_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0664);

	if (dest_fd == -1)
	{
		const int saved_errno = errno;
		close(src_fd);
		errno = saved_errno;
		err(1, "error : open destination file");
	}

	//reading from src and writing to dest
	ssize_t read_size = 0;
	while ( (read_size = read(src_fd, &buff, size_buff)) != 0) 
	{
		if (read_size == -1)
		{
			const int saved_errno = errno;
			close(src_fd);
			close(dest_fd);
			errno = saved_errno;
			err(1, "error : read source");
		} 

		//printf("read_size : %ld\n", read_size);

		if ( write(dest_fd, &buff, read_size) == -1 )
		{
			const int saved_errno = errno;
			close(src_fd);
			close(dest_fd);
			errno = saved_errno;
			err(1, "error : write destination");
		}
	}

	if ( close(src_fd) == -1 ) 
	{
		const int saved_errno = errno;
		close(dest_fd); // at least attempt to close destination
		errno = saved_errno;
		err(1, "error : close source file");
	}


	if ( close(dest_fd) == -1 ) 
	{
		err(1, "Error : close destination file");
	}

	exit(0);
}