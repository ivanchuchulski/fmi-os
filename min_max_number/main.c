
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

struct ByteNum
{
	uint8_t num;
};

int main(int argc, char* argv[])
{
	if (argc != 3)
		errx(5, "Error, two arguments needed!");

	struct stat s;
	stat(argv[2], &s);

	if (!S_ISREG(s.st_mode))
		errx(6, "Error, %s is not a regular file!", argv[2]);

	int fd= open(argv[2], O_RDONLY);
	if (fd == -1) err(7, "Error, cannot open file");

	if (strcmp("--print", argv[1]) == 0)
	{
		struct TwoByteNum current;

		while (read(fd, &current, sizeof(current)))
		{
			printf("%d\n", current.num);
		}

		exit(0);
	}

	if (strcmp(argv[1], "--big-byte") == 0)
	{
		struct ByteNum current;
		struct ByteNum biggest_byte;
		int counter = 1;

		while (read(fd, &current, sizeof(current)))
		{
			printf("%d\n", current.num);
			if (counter == 1)
			{
				biggest_byte = current;
				counter++;
			}

			else
				biggest_byte.num = (current.num > biggest_byte.num) ? current.num : biggest_byte.num;
		}

		printf("biggest byte is %d \n", biggest_byte.num);
		exit(0);

	}

	int counter = 1;
	struct TwoByteNum global_min;
	struct TwoByteNum global_max;
	struct TwoByteNum current;

	while (read(fd, &current, sizeof(current))) 
	{
		if (counter == 1)
		{
			global_min.num = current.num;
			global_max.num = current.num;
			counter++;
		}
		else
		{
			if (current.num < global_min.num)
				global_min.num = current.num;

			if (current.num > global_max.num)
				global_max.num = current.num;
		}

	}

	if (strcmp("--min", argv[1]) == 0)
		printf("global min is %d\n", global_min.num);

	else if (strcmp("--max", argv[1]) == 0)
		printf("global max is %d\n", global_max.num);

	else
		errx(19, "Error, %s is not a correct argument", argv[1]);

	exit(0);

}
