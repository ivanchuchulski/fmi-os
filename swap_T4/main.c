
#include <stdio.h>			//printf()

#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdlib.h>
#include <stdbool.h>

//program implementing wc
//TODO:not correct

void WriteFromTo(int src, int dest)
{
	char buff;

	while (read(src, &buff, 1))
	{
		write(dest, &buff, 1);
	}

}

int main(int argc, char* argv[])
{	
	if (argc != 3)
		errx(9, "1 argument needed!\n");

	int a;
	int b;
	int temp;

	a = open(argv[1], O_RDWR);
	if (a == -1)
		errx(10, "could not open first file!\n");

	b = open(argv[2], O_RDWR);
	if (b == -1)
	{
		errx(11, "could not open second file!\n");
		close(a);
	}

	temp = open("temp.txt", O_CREAT | O_RDWR, S_IRWXU, S_IRWXG);
	if (temp == -1)
	{
		errx(11, "could not open temp file!\n");
		close(a);
		close(b);
	}

	WriteFromTo(a, temp);
	lseek(a, 0, SEEK_SET);

	WriteFromTo(b, a);
	lseek(b, 0, SEEK_SET);
	lseek(temp, 0, SEEK_SET);

	WriteFromTo(temp, b);


	close(a);
	close(b);
	close(temp);

	exit(0);
}
