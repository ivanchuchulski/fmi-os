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
	// if (argc != 2) { errx(1, "error : #arguments should be two"); }

	// check if the command is executable
	// struct stat child_stat;
	// if ( stat(argv[1], &child_stat) == -1 ) { errx(2, "error : stat, %d", errno); }
	// if ( !(S_IXUSR & child_stat.st_mode) ) errx(1, "error : %s is not an executable file", argv[1]);

	while (1)
	{
		pid_t child_pid;
		int child_status;

		char* child_command_name = argv[1];

		// one way of building child arguments
		int argc_child = argc - 1;
		char* argv_child[argc_child + 1];

		for (int i = 1; i < argc; i++)
		{
			int child_argv_index = i - 1;
			argv_child[child_argv_index] = argv[i];
		}

		argv_child[argc_child] = NULL;

		// another way is this
		// char* const *argv_child_2 = &argv[1]; // &1[argv] just for fun

		// debugging info
		// print_argc_and_argv("parent arguments", argc, argv);
		// print_argc_and_argv("child_arguments", argc_child, argv_child);
		// print_argc_and_argv("child_arguments_2", argc_child, argv_child_2);

		child_pid = fork();

		if (child_pid == -1)
		{
			errx(2, "error : fork");
		}

		// child process
		if (child_pid == 0)
		{

			if (execv(child_command_name, argv_child) == -1)
			{
				err(2, "error : execv %s", child_command_name);
			}
		}

		// child_pid > 0 => parent process
		waitpid(child_pid, &child_status, 0);
		printf("parent process waited for child\n");

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

*/