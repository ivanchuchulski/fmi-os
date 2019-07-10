//06-hw-t2 : implementing cat with variable arguments provided on the command line

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{	
	if (argc < 2)
	{ 
		errx(1, "Error : at least 1 argument is needed!\n");
	}

	int fd;
	char buff;
	size_t size_buff = 1;

	for (int i = 1; i < argc; i++)
	{
		const char* current = argv[i];
		
		struct stat s;
		if ( stat(current, &s) == -1 ) err(1, "error : stat %s", current);
		if ( !S_ISREG(s.st_mode) ) err(1, "error : %s is not a regular file", current);

		fd = open(current, O_RDONLY);

		if (fd == -1)
		{
			err(1, "could not open file!");
		}

		while (read(fd, &buff, size_buff))
		{
			write(1, &buff, size_buff);		//here the first 1 denotes standart output
		}

		if (close(fd) == -1)
		{
			err(1, "close %s", current);
		}
	}

//	printf("\n");

	exit(0);
}
