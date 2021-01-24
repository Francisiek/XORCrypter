//Francisiek (C) 2021
#define VERSION 1.3f
#define AUTHOR "Francisiek"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


//enume for older standards
typedef enum {FALSE=0, TRUE=1} bool;
typedef unsigned long long int ULL;

//-h -u argument
void help(void)
{
	printf("\n\tUsage: xorc [-hvu] or [text file] [key file]\n\n");
	printf("\tPorogram crypts [text file] with [key file] by \n");
	printf("\tchanging it's content to crypted mess.\n");
	printf("\tTo decrypt, crypt file once more with the same key.\n");
	printf("\n\tARGS\n\t-h, -u shows this message\n");
	printf("\t-v show program info\n");
	printf("\tno args shows -v and -h\n\n");
}

//-v argument
void version(void)
{
	printf("\nXORCypter v. %.2f simple xor crypter\n", VERSION);
	printf("Contact: 'frx30340@protonmail.com'\n");
	printf("Github: 'https://github.com/Francisiek'\n\n");
	printf("Copyright (C) 2021 Francisiek\n");
	printf("This program is distributed under\n");
	printf("GNU General Public License v3.0 or later\n'https://www.gnu.org/licenses/gpl.html'\n\n");
	
}

//xcrypting func
char* xcrypt(const char* text, ULL N, const char* key, ULL M)
{
	char* out = (char*)malloc(N * sizeof(char));
	
	ULL x = 2;
	ULL pom = 0;
	for(ULL i=0; i<N; ++i)
	{
		out[i] = text[i];
		
		pom = i & 1;
		do
		{
			out[i] ^= key[pom];
			
			pom = (pom + x) % M;
		} while(pom != (i & 1));

		if(i & 1)
			++x;
	}
	
	return out;
}



int main(int argc, char* argv[])
{
	bool mode_c;
	int msg_fd, key_fd;
	struct stat msg_st, key_st;

	//default values
	mode_c = TRUE;
	msg_fd = key_fd = -1;

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

		//erros less than 2 arguments
		if(argc != 3)
		{
			printf("%s: bad syntax\n", argv[0]);
			return 4;
		}
	}
	else		//by default output help and version info
	{
		version();
		help();
		return 0;
	}
	
	//crytping
	if(mode_c)
	{
		msg_fd = open(argv[1], O_RDWR, 0);
		key_fd = open(argv[2], O_RDWR, 0);

		//performing text file
		if(msg_fd < 0 || fstat(msg_fd, &msg_st) < 0)
		{
			printf("%s: can't open file - %s\n", argv[0], argv[1]);
			return 1;
		}
	
		if(key_fd < 0 || fstat(key_fd, &key_st) < 0)
		{
			printf("%s: can't open file - %s\n", argv[0], argv[2]);
			return 1;
		}

		char* msg = (char*)malloc(msg_st.st_size);
		char* key = (char*)malloc(key_st.st_size);
		char *out;
		
		if(msg == NULL || key == NULL)
		{
			printf("%s: can't allocate memory\n", argv[0]);
			return 3;
		}
		
		//read msg
		if(read(msg_fd, msg, msg_st.st_size) != msg_st.st_size)
		{
			printf("%s: %s file reading failed\n", argv[0], argv[1]);
			return 2;
		}
		
		//read key
		if(read(key_fd, key, key_st.st_size) != key_st.st_size)
		{
			printf("%s: %s file reading failed\n", argv[0], argv[2]);
			return 2;
		}

		//calling xcrypt fun
		out = xcrypt(msg, msg_st.st_size, key, key_st.st_size);
		
		//writing coded text
		lseek(msg_fd, 0L, 0);
		if(write(msg_fd, out, msg_st.st_size) != msg_st.st_size)
		{
			printf("%s: %s file writing failed\n", argv[0], argv[1]);
			return 2;
		}		
		
		close(msg_fd);
		close(key_fd);

		free(msg);
		free(key);
		free(out);

	}	//end coding
	else		//if unknown mode set
	{
		printf("%s: bad syntax\n", argv[0]);
		return 4;
	}

	return 0;
}
