// 06-hw-t4 : copying and changing the separator character in /etc/passwd

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int src;
	int dest;
	char buff;
	size_t size_buff = 1;

	char quest = '?';
	char colon = ':';

	src = open("/etc/passwd", O_RDONLY);

	if (src == -1)	err(1, "error : open /etc/passwd");

	dest = open("./mypasswd", O_CREAT | O_TRUNC | O_WRONLY, 0664);

	if (dest == -1)
	{
		const int saved_errno = errno;
		close(src);
		errno = saved_errno;
		err(1, "error : open destination");
	}

	// printf("quest is : %c\n", quest);
	// printf("colon is : %c\n", colon);

	//reading from src and writing to dest
	while (read(src, &buff, size_buff))
	{
		if (buff == colon)
			write(dest, &quest, 1);
		else
			write(dest, &buff, size_buff);
	}

	if ( close(src) == -1 ) err(1, "error : close src");
	if ( close(dest) == -1 ) err(1, "error : close dest");

	exit(0);
}
