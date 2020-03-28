
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

struct Metadata	// total size = 8B
{
	uint8_t segm_type;	// 1B
	uint8_t params[7];	// 7 * 1B = 7B
};

union Essential	// union's size is always that of it's biggest element, thus 56B
{
	int8_t text_options[3][16];	// text options : 3 * 16 * 1B words = 48B  

	int32_t num_options[14];	// numerical options : 14 * 4B words = 56B

	int8_t byte_options[56];	// signle byte optnions : 56 * 1B words = 56B
};

struct Segment
{
	struct Metadata meta;
	union Essential ess;
};

// size_t func_count = 9;
// const char* functionalities[9] = {"-s", "-S", "-g", "-G", "-l", "-L", "-b", "-c", "-h"};

const size_t param_count = 15;
const char* parameters[15] = {"device_name", "rom_revision", "serial_number", 
							"bd_addr_part0", "bd_addr_part1", "bd_pass_part0", 
							"serial_baudrate", "audio_bitrate", "sleep_period", 
							"serial_parity", "serial_data_bit", "serail_stop_bit", 
							"bd_pass_part1", "rom_checksum_part0", "rom_checksum_part1"};

void DisplayHelp()
{


	const size_t usage_arr_size = 15;
	const char* usage_arr[] = {	"--Usage of the program--",
								"Provide <file> and one of the following options :",
								"-s <param> <val>\t: change <param>'s value to <val> and set the respective bit",
								"-S <param> <val>\t: change <param>'s value to <val> but don't set the respective bit",
								"-g <param>\t\t: view the <param> value is <file>, if the option is active",
								"-G <param>\t\t: view the <param> value is <file>, regardless if the option is active",
								"-l\t\t\t: view all active parameters",
								"-l <param_list>\t\t: view all active  parameters in <param_list>",
								"-L\t\t\t: view all active or inactive parameters",
								"-L <param_list>\t\t: view all active or inactive parameters in <param_list>",
								"-----------------------------------------------------------------------------",
								"Creation of a new configuration file ",
								"<new_cfg_name> -c 0 {t|n|b}, [1 {t|n|b}]...",
								"Display Help",
								"-h : display this help text" };
	const size_t examples_size = 3;
	const char* examples[] = {	"example :",
								"./conf cfg.bin -b device_name 0 -> deactivates the parameter device_name",
								"./conf cfg.bin -l device_name sleep_period -> view the values of the parameters, if they're active"
							};

	for (size_t i = 0; i < 2; i++)
	{
		printf("%s\n", usage_arr[i]);
	}

	for (size_t i = 2; i < usage_arr_size; i++)
	{
		printf("\t%s\n", usage_arr[i]);
	}

	int cnt = 1;
	printf("\n\tParameters are one of the following : \n");
	for (size_t i = 0; i < param_count; i++)
	{
		printf("\t\t%s", parameters[i]);
		if (cnt == 3)
		{
			printf("\n");
			cnt = 1;
		}
		else
			cnt++;
	}

	for (size_t i = 0; i < examples_size; i++)
	{
		printf("\t%s\n",examples[i]);
	}
}

bool IsRegularFile(const char* filename)
{
	struct stat buff;
	if ( stat(filename, &buff) == -1 ) 
	{
		err(1, "Error : stat %s", filename);
	}

	if (!S_ISREG(buff.st_mode))
	{
		return false;
	}
	else
	{
		return true;
	}
}


int main(int argc, char* argv[])
{
	switch (argc)
	{
		case 1 :
		{
			printf("Error : no arguments provided\n");
			DisplayHelp();	
			errx(1, "Error : Incorrect argument count");
		}

		case 2 :
		{
			if (strcmp("-h", argv[1]) == 0)
			{
				DisplayHelp();
				exit(0);
			}
			else
			{
				printf("Error : no such argument\n");
				DisplayHelp();
				errx(1, "Error : incorrect argument");
			}
		}

		case 3 :
		{
			if ( (strcmp("-l", argv[2]) == 0) || (strcmp("-L", argv[2]) == 0) )
			{
				if (!IsRegularFile(argv[1]))
				{
					errx(1, "Error : %s is not a regular file", argv[1]);		
				}

				int fd = open(argv[1], O_RDONLY);
				if (fd == -1)
				{
					err(1, "Error : open %s", argv[2]);			
				}	

				// bool only_used_params = true;
				// if (strcmp("-L", argv[2]) == 0)
				// {
				// 	only_used_params = false;
				// }

				close(fd);

			}
			else
			{
				printf("Error : incorrect arguments\n");
				DisplayHelp();		
				errx(1, "Error : Incorrect argument count");
			}
		}

		default :
			break;

	}

	//in the cases below we have at least 3 user arguments

	if (strcmp("-c", argv[2]) == 0)
	{
		if (argc == 3)
		{
			errx(5, "Error : -c must be provided with arguments to create segments");
		}

		int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if (fd == -1)
		{
			err(1, "Error : open %s", argv[2]);
		}

		close(fd);		

	}

	else if (strcmp("-b", argv[2]) == 0)
	{
		if (!IsRegularFile(argv[1]))
		{
			errx(1, "Error : %s is not a regular file", argv[1]);		
		}

		int fd = open(argv[1], O_RDWR);
		if (fd == -1)
		{
			err(1, "Error : open %s", argv[2]);			
		}

		close(fd);

	}

	else if ( (strcmp("-s", argv[2]) == 0) || (strcmp("-S", argv[2]) == 0) )
	{
		if (!IsRegularFile(argv[1]))
		{
			errx(1, "Error : %s is not a regular file", argv[1]);		
		}

		int fd = open(argv[1], O_RDWR);
		if (fd == -1)
		{
			err(1, "Error : open %s", argv[2]);			
		}

		// bool change_bit = true;
		// if (strcmp("-S", argv[2]) == 0)
		// {
		// 	change_bit = false;
		// }	

		close(fd);	

	}

	else if ( (strcmp("-l", argv[2]) == 0) || strcmp("-L", argv[2]) == 0 )
	{
		if (!IsRegularFile(argv[1]))
		{
			errx(1, "Error : %s is not a regular file", argv[1]);		
		}

		int fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			err(1, "Error : open %s", argv[2]);			
		}

		// bool only_used_params = true;
		// if (strcmp("-L", argv[2]) == 0)
		// {
		// 	only_used_params = false;
		// }	

		close(fd);	
	
	}
	
	else if ( (strcmp("-g", argv[2]) == 0) || strcmp("-G", argv[2]) == 0 )
	{
		if (!IsRegularFile(argv[1]))
		{
			errx(1, "Error : %s is not a regular file", argv[1]);		
		}

		int fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			err(1, "Error : open %s", argv[2]);			
		}

		// bool only_active_option = true;
		// if (strcmp("-L", argv[2]) == 0)
		// {
		// 	only_active_option = false;
		// }

		close(fd);

	}		

	else
	{
		printf("Error : incorrect arguments\n");
		DisplayHelp();
		exit (0);
	}


	//union size testing
	// union Essential un;

	// for (int i = 0; i < 3; i++)
	// {
	// 	for (int j = 0; j < 16; j++)
	// 	{
	// 		if (j == 15)
	// 			un.text_options[i][j] = '\0';
	// 		else
	// 			un.text_options[i][j] = 'a';
	// 	}
	// }

	// for (int i = 0; i < 3; i++)
	// {
	// 	for (int j = 0; j < 16; j++)
	// 	{
	// 		printf("%c", un.text_options[i][j]);
	// 	}
	// }

	// printf("\nsize of union is %ld\n", sizeof(un));

}
