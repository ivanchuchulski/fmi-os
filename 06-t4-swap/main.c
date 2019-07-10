// 06-t4 : swapping two files' contents
//TODO:not working if files are not the same size

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void WriteFromTo(int src, int dest)
{
	char buff;
	size_t size_buff = 1;

	while (read(src, &buff, size_buff))
	{
		write(dest, &buff, size_buff);
	}
}

int main(int argc, char* argv[])
{	
	if (argc != 3)
	{
		errx(1, "error : 1 argument needed");
	}

	int a;
	int b;
	int temp;

	a = open(argv[1], O_RDWR);
	if (a == -1) err(1, "error : open %s", argv[1]);

	b = open(argv[2], O_RDWR);
	if (b == -1)
	{
		const int saved_errno = errno;
		close(a);
		errno = saved_errno;
		err(1, "error : open %s", argv[2]);
	}

	temp = open("temp.txt", O_CREAT | O_TRUNC | O_RDWR, 0664);
	if (temp == -1)
	{
		const int saved_errno = errno;
		close(a);
		close(b);
		errno = saved_errno;
		err(1, "error : open temp.txt");
	}

	WriteFromTo(a, temp);
	if ( lseek(a, 0, SEEK_SET) == -1 ) err(1, "error : lseek %s", argv[1]);

	WriteFromTo(b, a);
	if ( lseek(b, 0, SEEK_SET) == -1 ) err(1, "error : lseek %s", argv[2]);
	if ( lseek(temp, 0, SEEK_SET) == -1 ) err(1, "error : lseek temp.txt");

	WriteFromTo(temp, b);

	if ( close(a) == -1) err(1, "error : close %s", argv[1]);
	if ( close(b) == -1) err(1, "error : close %s", argv[2]);
	if ( close(temp) == -1 ) err(1, "error : close temp.txt");

	exit(0);
}
