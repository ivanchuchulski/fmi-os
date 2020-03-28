// 06-t3 : implementing wc with one argument

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{	
	if (argc != 2)
	{
		errx(1, "error : 1 argument needed!");
	}

	int fd;
	char buff;
	size_t size_buff = 1;

	int rowcount = 0;
	int wordcount = 0;
	int charcount = 0;
	bool last_was_not_delimiter = false;

	fd = open(argv[1], O_RDONLY);

	if (fd == -1)
	{
		err(1, "error : open %s", argv[1]);
	}

	while (read(fd, &buff, size_buff))
	{
		charcount++;

		// 
		if (buff != ' ' && buff != '\n' && buff != '\t')
		{
			last_was_not_delimiter = true;
		}

		else 
		{
			if (last_was_not_delimiter)
				wordcount++;

			last_was_not_delimiter = false;

			if (buff == '\n')
				rowcount++;
		}

	//	write (1, &buff, size_buff);
	}

	// check if the file ended with a word sequence
	if (last_was_not_delimiter)
		wordcount++;

	printf("number of rows : %d, , number of words : %d, number of chars : %d\n",rowcount, wordcount, charcount);

	if (close(fd) == -1) 
	{
		err(1, "error : close %s", argv[1]);
	}

	exit(0);
}