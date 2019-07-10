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
	if (argc < 3) errx(1, "error : usage cp <file1>... <destname>");

	int src;
	int dest;
	char buff;
	size_t size_buff = 1;

	const char* dirname = argv[argc - 1];

	// check if the last argument is a directory
	struct stat dir_stat;
	if ( stat(dirname, &dir_stat) == -1 ) err(1, "error : stat %s", dirname);
	if ( !S_ISDIR(dir_stat.st_mode) ) errx(1, "error : %s is not a directory", dirname);

	for(int a = 1; a < argc - 1; a++)
	{

		char* sourcename = argv[a];

		src = open(sourcename, O_RDONLY);
		if (src == -1) err(1, "error : open %s", sourcename);

		int len = strlen(sourcename) + strlen(dirname); // not considering the \0

		char* destname = (char*)malloc(len + 1);

		if (destname == NULL) 
		{
			const int saved_errno = errno;
			close(src);
			errno = saved_errno;
			err(1, "error : malloc destname");
		}

		memset(destname, '\0', len + 1);

		strcat(destname, dirname);
		strcat(destname, "/");
		strcat(destname, sourcename);

		dest = open(destname,  O_CREAT | O_TRUNC | O_WRONLY, 0664);
		if (dest == -1)
		{
			const int saved_errno = errno;
			close(src);			
			errno = saved_errno;
			err(1, "error : open dest");
		}

		// printf("destname is : %s\n", destname);

		// reading from src and writing to dest
		while (read(src, &buff, size_buff))
		{
			write(dest, &buff, size_buff);
		}

		if ( close(src) == -1 ) err(1, "error : close %s", sourcename);
		if ( close(dest) == -1 ) err(1, "error : close %s", destname);
		free(destname);
	}

	exit(0);
}
