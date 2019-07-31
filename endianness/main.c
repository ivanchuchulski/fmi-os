// program to print simple text to the standart output and to check machine endianess

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <stdio.h>			//printf()
#include <stdlib.h>
#include <string.h>

int main()
{
	// printing a string
	for (int i = 0; i < 5; i++)
	{
		printf("Hello World!\n");
	}

	// endianness check
// 4B integer could be represented as  
//	01 23 45 67 -> big endian
//	or 67 45 23 01 -> little endian

	int num = 0x01234567;
	//int num1 = 0x67453201;
	char* dig = (char*) &num;
	printf("%x\n", *dig);	// printing the byte as hex number

	exit(0);
}
