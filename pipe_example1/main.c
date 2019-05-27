
#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdio.h>			//printf()
#include <stdlib.h>
#include <string.h>

//program implementing echo argv[1] | wc -c

const int READ = 0;
const int WRITE = 1;

int main(int argc, char* argv[])
{
	if (argc != 2)
		errx(1, "incorrect argument count, need 1 argument!");

	int p1[2];
	pipe(p1);

	int pid1 = fork();

	if (pid1 == -1)
		errx(55, "could not fork1!");

	if (pid1 > 0)
	{
		//proc1 code, doing echo
		close(p1[READ]);

		dup2(p1[WRITE], 1);
		close(p1[WRITE]);

		if(execlp("echo", "echo", argv[1], NULL) == -1)
			errx(2, "could not exec echo");
	}

	//proc2 code, doing wc -c
	close(p1[WRITE]);

	dup2(p1[READ], 0);
	close(p1[READ]);

	if (execlp("wc", "wc", "-c", NULL) == -1)
		err(3, "could not exec wc");


}
