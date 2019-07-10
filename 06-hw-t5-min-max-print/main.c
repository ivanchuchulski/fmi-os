// 06-hw-t5 : find the biggest or smallest uint_16 number in the hex dump or print each on a new line

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TwoByteNum
{
	uint16_t num;
};


int main(int argc, char* argv[])
{
	if (argc != 3) errx(1, "error : usage -> {--min|--max|--print} <hexdump>");

	struct stat s;
	if ( stat(argv[2], &s) == -1 ) err(1, "error : stat %s", argv[2]);
	if ( !S_ISREG(s.st_mode) ) errx(1, "error, %s is not a regular file", argv[2]);

	int fd = open(argv[2], O_RDONLY);
	if (fd == -1) err(1, "error, open %s", argv[2]);

	if (strcmp("--print", argv[1]) == 0)
	{
		struct TwoByteNum current;

		while (read(fd, &current, sizeof(current)))
		{
			printf("%d\n", current.num);
		}

		exit(0);
	}

	struct TwoByteNum global_min;
	struct TwoByteNum global_max;
	struct TwoByteNum current;

	read(fd, &current, sizeof(current));
	global_min.num = current.num;
	global_max.num = current.num;

	while (read(fd, &current, sizeof(current))) 
	{
		if (current.num < global_min.num)
			global_min.num = current.num;

		if (current.num > global_max.num)
			global_max.num = current.num;
	}

	if (strcmp("--min", argv[1]) == 0)
		printf("global min is %d\n", global_min.num);

	else if (strcmp("--max", argv[1]) == 0)
		printf("global max is %d\n", global_max.num);

	else
		errx(1, "error, %s is not a correct argument", argv[1]);

	if (close(fd) == -1)
	{
		err(1, "error : close %s", argv[2]);
	}

	exit(0);
}
