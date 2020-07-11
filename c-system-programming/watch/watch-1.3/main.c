#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_argc_and_argv(char* messege, int argc, char* const argv[]);

int main(int argc, char *const argv[])
{
	if (argc == 1) { errx(1, "error : #arguments should be at least two"); }

	while (1)
	{
		pid_t child_pid;
		char* child_command_name = argv[1];
		char* const *argv_child_2 = &argv[1]; // &1[argv] just for fun

		child_pid = fork();

		if (child_pid == -1)
		{
			perror("fork");
			exit(2);
		}

		// child process
		if (child_pid == 0)
		{
			if (execv(child_command_name, argv_child_2) == -1)
			{
				perror(child_command_name);
				exit(2);
			}
		}

		// child_pid > 0 => parent process
		int child_status;

		waitpid(child_pid, &child_status, 0);

		sleep(2);
	}

	exit(0);
}

void print_argc_and_argv(char* messege, int argc, char* const argv[])
{
	printf("%s\n", messege);

	printf("argc = %d\n", argc);

	for (int j = 0; j < argc; j++)
	{
		printf("%s\n", argv[j]);
	}
	
	printf("\n");
}