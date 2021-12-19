/*
	This file is part of XORCrypter.

    XORCrypter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with XORCrypter.  If not, see <https://www.gnu.org/licenses/>.
*/

//Francisiek (C) 2021
#define VERSION "1.5"
#define AUTHOR "Francisiek"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
	"Usage: xorc -[hvu] or [data_file] [key_file] -[io]\n"
	"-o save encrypted data in base64 format\n"
	"-i read input data in base64 format\n"
	"-h, -u shows help\n"
	"-v show program info\n"
	"no args shows -v and -h\n"
	"\nProgram encrypts [data file] with [key file] using\n"
	"xor operations so it is symetric encryption.\n");
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
	srand(time(NULL));

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

	int msg_fd, key_fd;
	char* msg_name, *key_name;
	struct stat msg_st, key_st;
	
	char* msg;
	char* key;
	int msglen, keylen, outlen;
	
	// options
	bool base64_input, base64_output;
	bool dont_save;

	const int MAX_INPUT = 4096;

	// default values
	msg_fd = key_fd = -1;
	msg_name = key_name = NULL;
	base64_input = base64_output = false;
	dont_save = false;
	msg = key = NULL;
	
	atexit(free_mem);

	//arguments managment
	if(argc > 1)
	{
		char opt;
		while((opt = getopt(argc, (char**)argv_copy, "-vhuois")) != -1)
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
				
				case 'o':
					base64_output = true;
					break;
				
				case 'i':
					base64_input = true;
					break;
				
				case 's':
					dont_save = true;
				break;

				case 1:
					if (msg_name)
						key_name = optarg;
					else
						msg_name = optarg;
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
	

	if (msg_name == NULL || *msg_name == '-')
	{
		msg_fd = 0;
		msglen = outlen = MAX_INPUT;

		msg = (char*)malloc(msglen);
		if (msg == NULL)
			handle_err(3, "%s: can't allocate memory\n", argv[0]);

		alloc_mem_add(msg);

		//read msg
		char b;
		int i = 0;
		do
		{
			if (read(0, &b, 1) == 0)
				break;

			msg[i++] = b;
		} while (b != EOF && i < MAX_INPUT);
		msglen = outlen = i;
		
		if (b == EOF)
		{
			msglen--;
			outlen--;
		}
	}
	else
	{
		msg_fd = open(msg_name, O_RDONLY, 0);
		if(msg_fd < 0 || fstat(msg_fd, &msg_st) < 0)
			handle_err(1, "%s: can't open file - %s\n", argv[0], msg_name);

		msglen = outlen = msg_st.st_size;

		msg = (char*)malloc(msglen);
		if (msg == NULL)
			handle_err(3, "%s: can't allocate memory\n", argv[0]);

		alloc_mem_add(msg);
		
		// read msg
		if (read(msg_fd, msg, msglen) != msglen)
			handle_err(2, "%s: reading of file %s failed", argv[0], msg_name);
	}

	if (key_name == NULL || *key_name == '-')
	{
		key_fd = 0;
		keylen = MAX_INPUT;

		key = (char*)malloc(keylen);
		if (key == NULL)
			handle_err(3, "%s: can't allocate memory\n", argv[0]);

		alloc_mem_add(key);

		//read key
		char b;
		int i = 0;
		do
		{
			if (read(0, &b, 1) == 0)
				break;

			key[i++] = b;
		} while (b != EOF && i < MAX_INPUT);
		keylen = i;
		if (b == EOF)
			keylen--;

	}
	else
	{
		key_fd = open(key_name, O_RDONLY, 0);
		if(key_fd < 0 || fstat(key_fd, &key_st) < 0)
			handle_err(1, "%s: can't open file - %s\n", argv[0], key_name);

		keylen = key_st.st_size;

		key = (char*)malloc(keylen);
		if (key == NULL)
			handle_err(3, "%s: can't allocate memory\n", argv[0]);

		alloc_mem_add(key);
		
		// read key
		if (read(key_fd, key, keylen) != keylen)
			handle_err(2, "%s: reading of file %s failed", argv[0], key_name);
	}


	char* output = (char*)malloc(msglen);
	if (output == NULL)
		handle_err(3, "%s: can't allocate memory\n", argv[0]);

	alloc_mem_add(output);
	
	if (base64_input)
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

	// erase data from memory
	for (int i = 0; i < msglen; ++i)
		msg[i] = rand();
	
	for (int i = 0; i < keylen; ++i)
		key[i] = rand();


	if (key_fd != 0)
		close(key_fd);
	
	if (dont_save || msg_fd == 0)
		msg_fd = 1;
	
	if (msg_fd != 1)
		msg_fd = open(argv[1], O_WRONLY | O_TRUNC, 0);

	if(msg_fd < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], msg_name);
	
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
		handle_err(2, "%s: %s file writing failed\n", argv[0], msg_name);
	
	if (msg_fd != 1) 
		close(msg_fd);

	//we're done!
	exit(EXIT_SUCCESS);
}

