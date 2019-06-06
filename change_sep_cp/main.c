
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//program implementing cp with two arguments
//TODO : it doesnt mind if the dir is <dirname>/ or without the slash in the argument string, maybe its not a bug

int main()
{
	int src;
	int dest;
	char buff;
	size_t sizeBuff = 1;
	char quest = '?';
	char colon = ':';

	src = open("/etc/passwd", O_RDONLY);

	if (src == -1)
		errx(5, "Error : Could not open /etc/passwd\n");

	dest = open("./mypasswd", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if (dest == -1)
	{
		close(src);			
		errx(11, "Error : Could not open destination file!\n");
	}

	printf("quest is : %c\n", quest);
	printf("colon is : %c\n", colon);

	//reading from src and writing to dest
	while (read(src, &buff, sizeBuff))
	{
		if (buff == colon)
			write(dest, &quest, 1);
		else
			write(dest, &buff, sizeBuff);
	}

	close(src);
	close(dest);


	exit(0);
}
