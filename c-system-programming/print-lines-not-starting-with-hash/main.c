#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

int main()
{
	const char *FILENAME = "b.txt";

	char buff;
	int fd = open(FILENAME, O_RDONLY);

	if (fd == -1)
	{
		perror("error");
		return -1;
	}

	bool at_beggining_of_line = true;
	int lenght = lseek(fd, 0, SEEK_END);

	lseek(fd, 0, SEEK_SET);

	for (int i = 0; i < lenght; i++)
	{
		read(fd, &buff, sizeof(buff));

		if (buff == '\n') 
		{
			at_beggining_of_line = true;
			write(1, &buff, sizeof(buff));
		}
		else if (buff == '#' && at_beggining_of_line)
		{
			//skip to the end of line
			while (1)
			{
				if (i >= lenght)
				{
					break;
				}
				
				char line_buff;

				read(fd, &line_buff, sizeof(line_buff));
				i++;

				if (line_buff == '\n')
				{
					at_beggining_of_line = true;
					break;
				}
			}
		}
		else 
		{
			at_beggining_of_line = false;
			write(1, &buff, sizeof(buff));
		}
	}

	if (close(fd) == -1)
	{
		perror("Error");
		return -1;
	}

	return 0;
}
