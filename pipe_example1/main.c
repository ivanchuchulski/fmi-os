
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <stdio.h>			
#include <stdlib.h>
#include <string.h>

//echo argv[1] | wc -c > argv[2]

const int READ = 0;
const int WRITE = 1;

int main(int argc, char* argv[])
{
	if (argc != 3) errx(5, "error : arguments must be <text> <file_to_write_output>");

	int output_fd;
	output_fd = open(argv[2], O_TRUNC | O_WRONLY);
	if (output_fd == -1) err(1, "error : could not open %s", argv[2]);

	int p1[2];
	pipe(p1);
	pid_t echo_pid;

	echo_pid = fork();
	if (echo_pid == -1) err(1, "error : fork echo");
	if (echo_pid > 0)
	{
		// echo
		close(p1[READ]);

		dup2(p1[WRITE], 1);
		close(p1[WRITE]);

		if (execlp("echo", "echo", argv[1], (char*)NULL) == -1) err(1, "error : execlp echo");
	}

	//wc -c
	close(p1[WRITE]);

	dup2(p1[READ], 0);
	close(p1[READ]);

	//writing to file instead of stdout
	dup2(output_fd, 1);
	close(output_fd);

	if (execlp("wc", "wc", "-c", (char*)NULL) == -1) err(1, "error : execlp wc -c");
}
