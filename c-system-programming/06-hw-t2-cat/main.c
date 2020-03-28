//06-hw-t2 : implementing cat with variable arguments provided on the command line

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

#define STDOUT 1

int main(int argc, char* argv[])
{	
	if (argc < 2)
	{ 
		errx(1, "error : at least 1 argument is needed\n\tusage : cat <file_name>...");
	}

	int fd;

	size_t size_buff = 256;
	char buff[size_buff];

	for (int i = 1; i < argc; i++)
	{
		const char* current_argv = argv[i];
		
		struct stat s;

		if ( stat(current_argv, &s) == -1 ) err(1, "error : stat %s", current_argv);
		if ( !S_ISREG(s.st_mode) ) err(1, "error : %s is not a regular file", current_argv);

		fd = open(current_argv, O_RDONLY);

		if (fd == -1)
		{
			err(1, "error : open %s", current_argv);
		}

		ssize_t read_size = 0;
		while ( (read_size = read(fd, &buff, size_buff)) != 0 )
		{
			if (read_size == -1)
			{
				const int saved_errno = errno;
				close(fd);
				errno = saved_errno;

				err(1, "error : read %s", current_argv);
			}

			if ( write(STDOUT, &buff, read_size) == -1 )
			{
				const int saved_errno = errno;
				close(fd);
				errno = saved_errno;

				err(1, "error : write STDOUT");
			}
		}

		if (close(fd) == -1)
		{
			err(1, "error : close %s", current_argv);
		}
	}

	exit(0);
}