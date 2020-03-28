// 06-hw-t6 : find the biggest byte in the hex dump

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ByteNum
{
	uint8_t num;
};

int main(int argc, char* argv[])
{
	if (argc != 2) errx(1, "error : usage -> <hex_dump>");

	const char* dumpfile = argv[1];

	struct stat s;
	if ( stat(dumpfile, &s) == -1 ) err(1, "error : stat %s", dumpfile);
	if ( !S_ISREG(s.st_mode) ) errx(1, "error, %s is not a regular file", dumpfile);

	int fd = open(dumpfile, O_RDONLY);
	if (fd == -1) err(1, "error, open %s", dumpfile);


	struct ByteNum biggest_byte;
	struct ByteNum current;

	read (fd, &current.num, sizeof(current));
	biggest_byte.num = current.num;

	while (read(fd, &current, sizeof(current)))
	{
		printf("%d\n", current.num);

		if (current.num > biggest_byte.num)
		{
			biggest_byte.num = current.num;
		}
	}

	printf("biggest byte is %d \n", biggest_byte.num);

	exit(0);
}
