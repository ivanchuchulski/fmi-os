// 06-t3 : implementing wc with one argument
//TODO : word count still not correct

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

	int rowcount = 0;
	int wordcount = 0;
	int charcount = 0;
	bool prev_was_letter = false;

	fd = open(argv[1], O_RDONLY);

	if (fd == -1)
	{
		err(1, "error : could not open file");
	}

	while (read(fd, &buff, sizeof(buff)))
	{
		charcount++;

		if (buff >= 'a' && buff <= 'Z')
		{
			prev_was_letter = true;
		}

		else 
		{
			if (prev_was_letter)
				wordcount++;

			prev_was_letter = false;

			if (buff == '\n')
				rowcount++;
		}

	//	write (1, &buff, sizeof(buff));
	}

	// initial version
	// while (read(fd, &buff, sizeof(buff)))
	// {
	// 	charcount++;

	// 	if (!inword)
	// 		wordcount++;

	// 	if (buff == ' ' || buff == '\n')
	// 	{
	// 		wordcount++;

	// 	}

	// 	if (buff != ' ')
	// 	{
	// 		inword = true;
	// 	}

	// 	if (buff == '\n')
	// 	{
	// 		rowcount++;
	// 		inword = true;
	// 	}

	// //	write (1, &buff, sizeof(buff));
	// }

	printf("number of rows : %d, , number of words : %d, number of chars : %d\n",rowcount, wordcount, charcount);

	if ( close(fd) == -1 ) err(1, "error : could not close file");

	exit(0);
}
