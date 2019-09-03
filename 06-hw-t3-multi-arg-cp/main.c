// 06-hw-t3 : implementing cp with random argument count
// the synopsis for this call is : cp <filename1> ... <filenameN> <destname>, where last argument is a directory
// TODO : it doesnt mind if destname has a lot of slashes after it or it doesn't

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
	if (argc < 3) errx(1, "error : not enough arguments\n\tusage : cp <file_name>... <dir_name>");

	int src_fd;
	int dest_fd;

	size_t size_buff = 256;
	char buff[size_buff];

	const char* dirname = argv[argc - 1];

	// check if the last argument is a directory
	struct stat dir_stat;

	if ( stat(dirname, &dir_stat) == -1 ) err(1, "error : stat %s", dirname);
	if ( !S_ISDIR(dir_stat.st_mode) ) errx(1, "error : %s is not a directory", dirname);

	for (int i = 1; i < argc - 1; i++)
	{
		char* sourcename = argv[i];
		char* destname = NULL;

		src_fd = open(sourcename, O_RDONLY);
		if (src_fd == -1) err(1, "error : open %s", sourcename);

		int len = strlen(sourcename) + strlen(dirname);

		destname = (char*)malloc(len + 1);

		if (destname == NULL) 
		{
			const int saved_errno = errno;
			close(src_fd);
			errno = saved_errno;

			err(1, "error : malloc destname");
		}

		memset(destname, '\0', len + 1);

		strcat(destname, dirname);
		strcat(destname, "/");
		strcat(destname, sourcename);
		
		// printf("destname is : %s\n", destname);

		dest_fd = open(destname,  O_CREAT | O_TRUNC | O_WRONLY, 0664);
		if (dest_fd == -1)
		{
			const int saved_errno = errno;
			close(src_fd);			
			errno = saved_errno;

			err(1, "error : open dest_fd");
		}


		ssize_t read_size = 0;
		while ( (read_size = read(src_fd, &buff, size_buff)) != 0 )
		{
			if (read_size == -1)
			{
				const int saved_errno = errno;
				close(src_fd);
				close(dest_fd);		
				errno = saved_errno;

				err(1, "eror : read %s", sourcename);
			}

			if ( write(dest_fd, &buff, read_size) == -1)
			{
				const int saved_errno = errno;
				close(src_fd);
				close(dest_fd);		
				errno = saved_errno;

				err(1, "eror : write %s", destname);				
			}
		}

		if ( close(src_fd) == -1 ) 
		{
			close(dest_fd);
			err(1, "error : close %s", sourcename);
		}


		if ( close(dest_fd) == -1 )
		{
			err(1, "error : close %s", destname);
		}

		free(destname);
	}

	exit(0);
}
