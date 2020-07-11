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
		char* const *argv_child_2 = &1[argv]; // &1[argv] just for fun

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
				// printf("%s : %s\n", child_command_name, strerror(errno));
				// err(3, "error : execv %s", child_command_name);
				// err(3, "%s : ", child_command_name);

				perror(child_command_name);
				exit(2);
			}
		}

		// child_pid > 0 => parent process

		// forcefully kill child
		// kill(child_pid, SIGKILL);

		int child_status;
		waitpid(child_pid, &child_status, 0);

		printf("parent process waited for child\n");

        if (WIFEXITED(child_status))
		{
			printf("child process exited normally(i.e. not being terminated by signal)\n");
			printf("child finished with %d\n", WEXITSTATUS(child_status));
		}

        if (WIFSIGNALED(child_status)) 
		{
			printf("child process terminated by signal\n");
		}

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

/*
if main is called with : 
	$ ./main ls -a -l -h

then the child program must be called like this
	$ ls -a -l -h

in $ ./main a b c
	printf("%c\n", **(argv)); 			-> .
	printf("%c\n", **(argv + 1));		-> a
	printf("%c\n", **(argv + 2));		-> b

one way of checking if a command(a file) is executable
	struct stat child_stat;
	if ( stat(argv[1], &child_stat) == -1 ) { errx(2, "error : stat, %d", errno); }
	if ( !(S_IXUSR & child_stat.st_mode) ) { errx(3, "error : %s is not an executable file", argv[1]); }

*/