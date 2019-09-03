// echo argv[1] | wc -c > argv[2]

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
	if (argc != 3) errx(1, "error : arguments must be <text> <file_to_write_output>");

	int output_fd;
	output_fd = open(argv[2], O_TRUNC | O_WRONLY);
	if (output_fd == -1) err(1, "error : open %s", argv[2]);

	int p1[2];
	if ( pipe(p1) == -1 ) err(1, "error : pipe p1");
	pid_t echo_pid;

	echo_pid = fork();
	if (echo_pid == -1) err(1, "error : fork echo");
	if (echo_pid > 0)
	{
		// echo
		if ( close(p1[READ]) == -1) err(1, "error : close p1[0]");

		if ( dup2(p1[WRITE], 1) == -1 ) err(1, "error : dup2 p1[1], 1");
		if ( close(p1[WRITE]) == -1 ) err(1, "error : close p1[1]");

		if (execlp("echo", "echo", argv[1], (char*)NULL) == -1) 
		{
			const int saved_erro = errno;
			close(output_fd);
			errno = saved_erro;
			err(1, "error : execlp echo");
		}
	}

	//wc -c
	if ( close(p1[WRITE]) == -1 ) err(1, "error : close p1[1]");

	if ( dup2(p1[READ], 0) == -1 ) err(1, "error : dup2 p1[0], 0");
	if ( close(p1[READ]) == -1 ) err(1, "error : close p1[0]");

	//writing to file instead of stdout
	if ( dup2(output_fd, 1) == -1 ) err(1, "error : dup2 %s, 1", argv[2]);
	if ( close(output_fd) == -1 ) err(1, "error : close %s", argv[2]);

	if (execlp("wc", "wc", "-c", (char*)NULL) == -1)
	{
		const int saved_erro = errno;
		close(output_fd);
		errno = saved_erro;		
		err(1, "error : execlp wc -c");
	}
}
