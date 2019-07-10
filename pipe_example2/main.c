// head argv[1] | grep argv[2] | wc -l 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int READ = 0;
const int WRITE = 1;

int main(int argc, char* argv[])
{
	if (argc != 3) errx(1, "error : arguments must be <file> <pattern>");

	pid_t head_pid;
	pid_t grep_pid;

	int p1[2];
	int p2[2];

	if ( pipe(p1) == -1 ) err(1, "error : pipe p1");

	head_pid = fork();
	if (head_pid == -1) err(1, "error : fork head");
	if (head_pid > 0)
	{
		//proc1, head
		if ( close(p1[READ]) == -1 ) err(1, "error : close p1[READ]");

		if ( dup2(p1[WRITE], 1) == -1 ) err(1, "error : dup2 p1[WRITE]");
		if ( close(p1[WRITE]) == -1 ) err(1, "error : close p1[WRITE]");

		if ( execlp("head", "head", argv[1], (char*)NULL) == -1 ) err(1, "error : execlp head");
	}

	if ( close(p1[WRITE]) == -1 ) err(1, "error : close p1[1]");

	if ( pipe(p2) == -1 ) err(1, "error : pipe p2");

	grep_pid = fork();
	if (grep_pid == -1)	err(1, "error : fork grep");
	if (grep_pid > 0)
	{
		//proc2, grep
		if ( dup2(p1[READ], 0) == -1 ) err(1, "error : dup2 p1[0], 0");
		if ( close(p1[READ]) == -1 ) err(1, "error : close p1[0]");

		if ( close(p2[READ]) == -1 ) err(1, "error : close p2[0]");

		if ( dup2(p2[WRITE], 1) == -1 ) err(1, "error : dup2 p2[1], 1");
		if ( close(p2[WRITE]) == -1 ) err(1, "error : close p2[1]");

		if (execlp("grep", "grep", argv[2], (char*)NULL) == -1) err(1, "error : execlp grep");
	}

	//proc3, wc -l
	if ( close(p2[WRITE]) == -1 ) err(1, "error : close p2[1]");

	if ( dup2(p2[READ], 0) == -1 ) err(1, "error : dup2 p2[0], 0");
	if ( close(p2[READ]) == -1 ) err(1, "error : close p2[0]");

	if (execlp("wc", "wc", "-l", (char*)NULL) == -1) err(1, "error : execlp wc -l");
}
