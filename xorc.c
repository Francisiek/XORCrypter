//Francisiek (C) 2021
#define VERSION 1.3f
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
typedef unsigned long long int ULL;

//global variable to store argv[0]
char* prog_name;
void* alloc_mem[3];

//-h -u argument
void help(void)
{
	printf("Usage: xorc [-hvu] or [text_file] [key_file]\n");
	printf("\n-h, -u\tshows help\n");
	printf("-v\tshow program info\n");
	printf("no args shows -v and -h\n");
	printf("\nPorogram crypts text_file with key_file.\n");
	printf("To decrypt, crypt file once more with the same key.\n\n");
}

//-v argument
void version(void)
{
	printf("XORCypter v%.2f simple xor crypter\n", VERSION);
	printf("Contact: 'frx30340@protonmail.com'\n");
	printf("Github: 'https://github.com/Francisiek'\n\n");
	printf("Copyright (C) 2021 Francisiek\n");
	printf("This program is distributed under\n");
	printf("GNU General Public License v3.0 or later\n");
	printf("'https://www.gnu.org/licenses/gpl.html'\n");
	
}

void free_mem(void)
{
	for(int i = 0; i < 3; i++)
		if(alloc_mem[i])
			free(alloc_mem[i]);
}

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
	exit(rt);
}


//xcrypting function
char* xcrypt(const char* text, ULL text_size, const char* key, ULL key_size)
{
	char* out = (char*)malloc(text_size);
	if(out == NULL)
		handle_err(3, "%s: can't allocate memory\n", prog_name);
	
	alloc_mem[2] = out;
	
	ULL cur_jmp = 2;
	ULL indx = 0;

	for(ULL i = 0; i < text_size; ++i)
	{
		out[i] = text[i];
		
		indx = i & 1;
		do
		{
			out[i] ^= key[indx];
			
			indx = (indx + cur_jmp) % key_size;
		} while(indx != (i & 1));

		if(i & 1)
			++cur_jmp;
	}
	
	return out;
}

int main(int argc, char* argv[])
{
	int msg_fd, key_fd;
	struct stat msg_st, key_st;

	//default values
	msg_fd = key_fd = -1;
	prog_name = argv[0];
	
	atexit(free_mem);

	//arguments
	if(argc > 1)
	{
		if(!strcmp(argv[1], "-v"))
		{
			version();
			return 0;
		}

		if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "-u"))
		{
			help();
			return 0;
		}
		
		//error bad args
		if(argc != 3)
			handle_err(4, "%s: bad syntax\n", argv[0]);

	}
	else		//by default output help and version info
	{
		help();
		version();
		return 0;
	}
	
	//crytping
	msg_fd = open(argv[1], O_RDWR, 0);
	key_fd = open(argv[2], O_RDWR, 0);

	//getting files sizes
	if(msg_fd < 0 || fstat(msg_fd, &msg_st) < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], argv[1]);

	if(key_fd < 0 || fstat(key_fd, &key_st) < 0)
		handle_err(1, "%s: can't open file - %s\n", argv[0], argv[2]);

	char* msg = (char*)malloc(msg_st.st_size);
	char* key = (char*)malloc(key_st.st_size);
	char* out;
	
	if(msg == NULL || key == NULL)
		handle_err(3, "%s: can't allocate memory\n", argv[0]);
	
	alloc_mem[0] = msg;
	alloc_mem[1] = key;

	//read msg
	if(read(msg_fd, msg, msg_st.st_size) != msg_st.st_size)
		handle_err(2, "%s: %s file reading failed\n", argv[0], argv[1]);
	
	//read key
	if(read(key_fd, key, key_st.st_size) != key_st.st_size)
		handle_err(2, "%s: %s file reading failed\n", argv[0], argv[2]);

	//calling xcrypt fun
	out = xcrypt(msg, msg_st.st_size, key, key_st.st_size);
	
	//writing to file
	lseek(msg_fd, 0L, 0);

	if(write(msg_fd, out, msg_st.st_size) != msg_st.st_size)
		handle_err(2, "%s: %s file writing failed\n", argv[0], argv[1]);
	
	exit(0);
}

