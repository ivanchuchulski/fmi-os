
#include <stdio.h>			//printf()

#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdlib.h>
#include <string.h>

//program implementing cp with two arguments
//TODO : it doesnt mind if the dir is <dirname>/ or without the slash in the argument string

int main(int argc, char* argv[])
{
	int src;
	int dest;
	char buff;

	if (argc == 1)
	{
		errx(5, "Error : at least 1 argument needed!\n");
	}

	for(int a = 0; a < argc - 2; a++)
	{

		src = open(argv[a + 1], O_RDONLY);

		if (src == -1)
			errx(10, "Error : Could not open source file!\n");

		int len = strlen(argv[argc - 1]) + strlen(argv[a + 1]);
		char* destname = (char*)malloc(len + 1);

		if (destname == NULL)
			errx(99, "Error : not enough memory to allocate for the strings\n");

		memset(destname, '\0', len + 1);

		strcat(destname, argv[argc - 1]);
		strcat(destname, "/");
		strcat(destname, argv[a + 1]);

		dest = open(destname, O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

		printf("destname is : %s\n", destname);
		printf("dest is : %d\n", dest);

		if (dest == -1)
		{
			close(src);			
			errx(11, "Error : Could not open destination file!\n");

		}

		//reading from src and writing to dest
		while (read(src, &buff, sizeof(buff)))
		{
			write(dest, &buff, sizeof(buff));
		}

		close(src);
		close(dest);
		free(destname);
	}

	exit(0);
}
