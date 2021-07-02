//Francisiek (C) 2021
#define VERSION "1.4"
#define AUTHOR "Francisiek"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#include "base64/base64.h"
#include "handle_err.h"
#include "mem.h"
#include "xorc.h"

//-h -u argument
void help(void)
{
	printf("%s",
	"Usage: xorc -[hvu] or [text_file] [key_file]\n"
	"\n-h, -u\tshows help\n"
	"-v\tshow program info\n"
	"no args shows -v and -h\n"
	"\nProgram crypts text_file with key_file.\n"
	"To decrypt, crypt file once more with the same key.\n\n"
	);
}

//-v argument
void version(void)
{
	printf("%s",
	"XORCrypter v"VERSION" simple xor crypter\n"
	"Contact: 'frx30340@protonmail.com'\n"
	"Github: 'https://github.com/Francisiek'\n\n"
	"Copyright (C) 2021 Francisiek\n"
	"This program is distributed under\n"
	"GNU General Public License v3.0 or later\n"
	"'https://www.gnu.org/licenses/gpl.html'\n"
	);
	
}

int main(int argc, char* argv[])
{

	/*
	getopt swaps orded of argv so
	we will give it a copy
	*/
	char* argv_copy[argc + 1];
	for(int i = 0; i < argc; ++i)
	{
		argv_copy[i] = (char*)malloc(strlen(argv[i]) + 1);
		if(argv_copy[i] == NULL)
			handle_err(3, "%s: can't allocate memory\n", argv[0]);

		strcpy(argv_copy[i], argv[i]);
	}

	/*
	for(int i = 0; i < argc; ++i)
	{
		fprintf(stderr, "%s\n", argv[i]);
	}
	*/

	int msg_fd, key_fd;
	int msglen, keylen, outlen;
	struct stat msg_st, key_st;
	bool base64_input, base64_output;

	//default values
	msg_fd = key_fd = -1;
	base64_input = base64_output = false;
	atexit(free_mem);

	//arguments managment
	if(argc > 1)
	{
		char opt;
		while((opt = getopt(argc, (char**)argv_copy, "vhuoimk")) != -1)
		{
			switch(opt)
			{
				case 'v':
					version();
					return 0;
					break;

				case 'h':
					help();
					return 0;
					break;
				
				case 'u':
					help();
					return 0;
					break;
				
				case 'm':
					
					break;

				case 'k':
					break;
				
				case 'o':
					base64_output = true;
					break;
				
				case 'i':
					base64_input = true;
					break;
				
				default:
					handle_err(4, "");
			}
		}

		if(argc <= 2)
			handle_err(4, "%s: bad syntax\n", argv[0]);
	}
	//by default output help and version info
	else
	{
		help();
		version();
		return 0;
	}

	/*
	for(int i = 0; i < argc; ++i)
	{
		printf("%s\n", argv_copy[i]);
	}
	*/

	//opening files
	msg_fd = open(argv[1], O_RDONLY, 0);
	key_fd = open(argv[2], O_RDONLY, 0);

	//getting files sizes
	if(msg_fd < 0 || fstat(msg_fd, &msg_st) < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], argv[1]);

	if(key_fd < 0 || fstat(key_fd, &key_st) < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], argv[2]);
	
	msglen = outlen = msg_st.st_size;
	keylen = key_st.st_size;

	char* msg = (char*)malloc(msglen);
	char* key = (char*)malloc(keylen);
	char* output = (char*)malloc(msglen);
	
	if(msg == NULL || key == NULL || output == NULL)
		handle_err(3, "%s: can't allocate memory\n", argv[0]);
	
	//save those pointers
	alloc_mem_add(msg);
	alloc_mem_add(key);
	alloc_mem_add(output);

	//read msg
	if(read(msg_fd, msg, msglen) != msglen)
		handle_err(2, "%s: %s file reading failed\n", argv[0], argv[1]);
	
	//read key
	if(read(key_fd, key, keylen) != keylen)
		handle_err(2, "%s: %s file reading failed\n", argv[0], argv[2]);
	
	close(msg_fd);
	
	if(base64_input)
	{
		char* binary = (char*)malloc(msglen);	
		msglen = outlen = b64_decode((unsigned char*)msg, msglen, (unsigned char*)binary);
		
		if(binary == NULL)
			handle_err(3, "%s: can't allocate memory\n", argv[0]);
		
		free(msg);
		alloc_mem_del(msg);
		alloc_mem_add(binary);
		msg = binary;
	}

	//calling xcrypt function
	xcrypt(msg, msglen, key, keylen, output);

	//writing to file
	//lseek(msg_fd, 0L, 0);
	
	msg_fd = open(argv[1], O_WRONLY | O_TRUNC, 0);

	if(msg_fd < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], argv[1]);
	
	if(base64_output)
	{
		char* plaintext = (char*)malloc( b64e_size(msglen) );
		outlen = b64_encode((unsigned char*)output, msglen, (unsigned char*)plaintext);
		
		if(plaintext == NULL)
			handle_err(3, "%s: can't allocate memory\n", argv[0]);
		
		//because b64_encode returns size of string with null byte.
		//outlen--;

		free(output);
		alloc_mem_del(output);
		alloc_mem_add(plaintext);
		output = plaintext;
	}
	
	if(write(msg_fd, output, outlen) != outlen)
		handle_err(2, "%s: %s file writing failed\n", argv[0], argv[1]);
	
	close(msg_fd);
	close(key_fd);

	//we're done!
	exit(EXIT_SUCCESS);
}

