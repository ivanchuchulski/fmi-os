
#include <sys/types.h>		//open(), lseek()
#include <sys/stat.h>		//open()
#include <fcntl.h>			//open()
#include <unistd.h>			//close(), read(), write(), lseek()
#include <err.h>			//err()

#include <stdio.h>			//printf()
#include <stdlib.h>
#include <string.h>

//program head argv[1] | grep argv[2] | wc -l 

const int READ = 0;
const int WRITE = 1;


int main(int argc, char* argv[])
{
	if (argc != 3) errx(99, "incorrect argument count, need 2 arguments!");

	int p1[2];
	pipe(p1);
	int pid1 = fork();

	if (pid1 == -1) errx(55, "could not fork1!");
	if (pid1 > 0)
	{
		//proc1 code, head
		close(p1[READ]);

		dup2(p1[WRITE], 1);
		close(p1[WRITE]);

		if(execlp("head", "head", argv[1], (char*)NULL) == -1) errx(1, "could not exec head");
	}


	close(p1[WRITE]);

	int p2[2];
	pipe(p2);
	int pid2 = fork();

	if (pid2 == -1)	errx(56, "could not fork2!");
	if (pid2 > 0)
	{
		//proc2 code, grep

		dup2(p1[READ], 0);
		close(p1[READ]);

		close(p2[READ]);

		dup2(p2[WRITE], 1);
		close(p2[WRITE]);

		if (execlp("grep", "grep", argv[2], (char*)NULL) == -1) err(2, "could not exec grep");
	}

	//proc3, wc -l
	close(p2[WRITE]);

	dup2(p2[READ], 0);
	close(p2[READ]);

	if (execlp("wc", "wc", "-l", (char*)NULL) == -1) err(3, "could not exec wc");


}
