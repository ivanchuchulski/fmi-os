
#include <stdio.h>			//printf()

#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdlib.h>
#include <stdbool.h>

//program implementing wc
//TODO:word count still not correct

int main(int argc, char* argv[])
{	
	if (argc != 2)
		errx(9, "1 argument needed!\n");

	int fd;
	int rowcount = 0;
	int wordcount = 0;
	int charcount = 0;
	char buff;
	bool inword = true;

	fd = open(argv[1], O_RDONLY);

	if (fd == -1)
		errx(10, "could not open file!\n");

	while (read(fd, &buff, sizeof(buff)))
	{
		charcount++;

		if (!inword)
			wordcount++;

		if (buff == ' ' || buff == '\n')
		{
			wordcount++;

		}

		if (buff != ' ')
		{
			inword = true;
		}

		if (buff == '\n')
		{
			rowcount++;
			inword = true;
		}

	//	write (1, &buff, sizeof(buff));
	}

	printf("number of rows : %d, , number of words : %d, number of chars : %d \n",rowcount, wordcount, charcount);

	close(fd);

	exit(0);
}
