//Francisiek (C) 2021
#define VERSION "1.4"
#define AUTHOR "Francisiek"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


//why not your own bool?
typedef enum {FALSE = 0, TRUE = 1} bool;

//global variable to store argv[0] and table of allocated pointers
char* prog_name;
void* alloc_mem[3];

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

//free memory from allocated pointers table
void free_mem(void)
{
	for(int i = 0; i < 3; i++)
		if(alloc_mem[i])
			free(alloc_mem[i]);
}

//print errors
void handle_err(int rt, char* format, ...)
{
	va_list args;
	va_start(args, format);

	while(*format)
	{
		if(strncmp(format, "%d", 2) == 0)
		{
			fprintf(stderr, "%d", va_arg(args, int));
			format += 2;
		}
		else if(strncmp(format, "%s", 2) == 0)
		{
			fprintf(stderr, "%s", va_arg(args, char*));
			format += 2;
		}
		else
		{
			fprintf(stderr, "%c", *format);
			format++;
		}
	}

	va_end(args);

	free_mem();
	exit(rt);
}


//xcrypting function
char* xcrypt(const char* text, size_t text_size, const char* key, size_t key_size)
{
	char* output = (char*)malloc(text_size);
	if(output == NULL)
		handle_err(3, "%s: can't allocate memory\n", prog_name);
	
	//save pointer to table
	alloc_mem[2] = output;
	
	unsigned int cur_jmp = 2;
	unsigned int indx = 0;
	
	//some magic
	for(size_t i = 0; i < text_size; ++i)
	{
		output[i] = text[i];
		
		indx = i & 1;
		do
		{
			output[i] ^= key[indx];
			
			indx = (indx + cur_jmp) % key_size;
		} while(indx != (i & 1));

		if(i & 1)
			++cur_jmp;
	}
	
	return output;
}

int main(int argc, char* argv[])
{
	int msg_fd, key_fd;
	struct stat msg_st, key_st;

	//default values
	msg_fd = key_fd = -1;
	prog_name = argv[0];
	
	atexit(free_mem);

	//arguments managment
	if(argc > 1)
	{
		char opt;
		while((opt = getopt(argc, argv, "vhu")) != -1)
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
	
	//opening files
	msg_fd = open(argv[1], O_RDWR, 0);
	key_fd = open(argv[2], O_RDWR, 0);

	//getting files sizes
	if(msg_fd < 0 || fstat(msg_fd, &msg_st) < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], argv[1]);

	if(key_fd < 0 || fstat(key_fd, &key_st) < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], argv[2]);

	char* msg = (char*)malloc(msg_st.st_size);
	char* key = (char*)malloc(key_st.st_size);
	char* output;
	
	if(msg == NULL || key == NULL)
		handle_err(3, "%s: can't allocate memory\n", argv[0]);
	
	//save those pointers
	alloc_mem[0] = msg;
	alloc_mem[1] = key;

	//read msg
	if(read(msg_fd, msg, msg_st.st_size) != msg_st.st_size)
		handle_err(2, "%s: %s file reading failed\n", argv[0], argv[1]);
	
	//read key
	if(read(key_fd, key, key_st.st_size) != key_st.st_size)
		handle_err(2, "%s: %s file reading failed\n", argv[0], argv[2]);

	//calling xcrypt function
	output = xcrypt(msg, msg_st.st_size, key, key_st.st_size);
	
	//writing to file
	lseek(msg_fd, 0L, 0);

	if(write(msg_fd, output, msg_st.st_size) != msg_st.st_size)
		handle_err(2, "%s: %s file writing failed\n", argv[0], argv[1]);
	
	//we're done!
	exit(0);
}

