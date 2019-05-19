
#include <stdio.h>			//printf()

#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdlib.h>

//system calls
/*
	int open(const char* pathaname, int flags, mode_t mode);
	int close(int fd);
	ssize_t read(int fd, void* buff, size_t count);
	ssize_t write(int fd, const void* buff, size_t count);
	off_t lseek(int fd, off_t offset, int whence);

*/

//library calls
/*
	int printf(const char* format, ...);
	void errx(int eval, const char* fmt, ...);
*/


//program implementing cp

int main()
{
	int fd_src;
	int fd_dest;
	char buff;

	fd_src = open("foo.txt", O_RDONLY);

	if (fd_src == -1)
	{
		errx(10, "Could not open source file!\n");
	}

	fd_dest = open("bar.txt", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);

	if (fd_dest == -1)
	{
		errx(11, "Could not open destination file!\n");
		close(fd_src);
	}

	while (read(fd_src, &buff, sizeof(buff)))
	{
		write(fd_dest, &buff, sizeof(buff));
	}

	close(fd_src);
	close(fd_dest);

	exit(0);
}
