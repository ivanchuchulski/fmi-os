// cut -d: -f7 /etc/passwd | sort | uniq -c | sort -n
// the most used shells on the system

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int READ = 0;
const int WRITE = 1;

int main()
{
	pid_t cut_pid;
	pid_t sort_pid;
	pid_t uniq_pid;

	int p1[2];
	int p2[2];
	int p3[2];

	if ( pipe(p1) == -1 ) err(1, "error : pipe p1");

	cut_pid = fork();
	if (cut_pid == -1) err(1, "error : fork cut");
	if (cut_pid > 0)
	{
		//proc1, cut -d: -f7 /etc/passwd
		if ( close(p1[READ]) == -1 ) err(1, "error : close p1[READ]");

		if ( dup2(p1[WRITE], 1) == -1 ) err(1, "error : dup2 p1[WRITE] 1 cut");
		if ( close(p1[WRITE]) == -1 ) err(1, "error : close p1[WRITE])");

		if ( execlp("cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL) == -1 ) err(1, "error : execlp cut");
	}

	if ( close(p1[WRITE]) == -1 ) err(1, "error : close p1[1]");

	if ( pipe(p2) == -1 ) err(1, "error : pipe p2");

	sort_pid = fork();
	if (sort_pid == -1)	err(1, "error : fork sort");
	if (sort_pid > 0)
	{	
		if ( dup2(p1[READ], 0) == -1 ) err(1, "error : dup2 p1[0], 0");
		close(p1[READ]);

		//proc2, sort
		if ( close(p2[READ]) == -1 ) err(1, "error : close p2[0]");

		if ( dup2(p2[WRITE], 1) == -1 ) err(1, "error : dup2 p2[1], 1");
		close(p2[WRITE]);

		if (execlp("sort", "sort", (char*)NULL) == -1) err(1, "error : execlp sort");	//- in sort means read from stdin
	}

	if ( close (p2[WRITE]) == -1 ) err(1, "error : close p2[1]");

	if ( pipe(p3) == -1 ) err(1, "error : pipe p3");

	uniq_pid = fork();
	if (uniq_pid == -1) err(1, "error, fork uniq");
	if (uniq_pid > 0)
	{
		//proc3, uniq -c
		if ( dup2(p2[READ], 0) == -1 ) err(1, "error : dup2 p2[0], 0");
		if ( close(p2[READ]) == -1 ) err(1, "error : close p2[0]");

		if ( close(p3[READ]) == -1 ) err(1, "error : close p3[0]");

		if ( dup2(p3[WRITE], 1) == -1 ) err(1, "error : dup2 p3[1], 1");
		if ( close(p3[WRITE]) == -1 ) err(1, "error : close p3[1]");

		if (execlp("uniq", "uniq", "-c", (char*)NULL) == -1) err(1, "error : execlp uniq -c");
	}

	//proc4, sort -n
	if ( close(p3[WRITE]) == -1 ) err(1, "error : close p3[1]");

	if ( dup2(p3[READ], 0) == -1 ) err(1, "error : dup2 p3[0], 0");
	if ( close(p3[READ]) == -1 ) err(1, "error : close p3[0]");

	if (execlp ("sort", "sort", "-n", (char*)NULL) == -1) err(1, "error : execlp sort -n");	
}
