//Francisiek (C) 2020
#define VERSION 1.2f
#define AUTHOR "Francisiek"
#define LOG printf("log\n");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



//enume for older standards
typedef enum {FALSE=0, TRUE=1} bool;

//-h -u argument
void help(void)
{
	printf("\n\tUsage: xorc [arg] [msg file] [key file]\n\n");
	printf("\t-c crypt the text file and change it's content to crypted binary numbers.\n\n");
	printf("\t-d decrypt the binary file. Change it's content back to original text.\n\n");
	printf("\t-h, -u shows this message\n");
	printf("\t-v show program info\n\n");
}

//-v argument
void version(void)
{
	printf("\nXORCypter v. %.2f simple xor crypting files program\n", VERSION);
	printf("Contact: 'francisiek.yt@gmail.com'\n");
	printf("Github: 'https://github.com/Francisiek'\n\n");
	printf("Copyright (C) 2020 Francisiek\n");
	printf("This program is distributed under\n");
	printf("GNU General Public License v3.0 or later\n'https://www.gnu.org/licenses/gpl.html'\n\n");
	
}

//crypting func
char* crypt(const char* text, const char* key)
{
	int N = strlen(text);
	int M = strlen(key);
	
	char* out = malloc(N * sizeof(char));
	
	int x = 2;
	int pom = 0;
	for(int i=0; i<N; ++i)
	{
		out[i] = text[i];
		
		pom = i%2;
		do
		{
			out[i] ^= key[pom];
			
			pom += x;
			pom %= M;
		} while(pom != i%2);

		if(i%2)
			++x;
	}
	
	return out;
}

//decrypting func
char* dcrypt(const char* text, const char* key)
{
	int N = strlen(text);
	int M = strlen(key);

	char* out = malloc(N * sizeof(char));
	
	int x = 2;
	int pom = 0;
	for(int i=0; i<N; ++i)
	{
		out[i] = text[i];
		
		pom = i%2;
		do
		{
			out[i] ^= key[pom];
		
			pom += x;
			pom %= M;
		} while(pom != i%2);
		
		if(i%2)
			++x;
	}
	
	return out;
}


int main(int argc, char* argv[])
{
	bool mode_c, mode_d;
	int msg_fd, key_fd;
	struct stat msg_st, key_st;

	//default values
	mode_c = mode_d = FALSE;
	msg_fd = key_fd = NULL;

	//arguments
	if(argc > 1)
	{
		int i = 1;
		
		while(argv[1][i])
		{
			//setting modes
			switch(argv[1][i])
			{
				case 'c': mode_c = TRUE; break;
				case 'd': mode_d = TRUE; break;
				case 'v': version(); return 0;  break;
				case 'h': help(); return 0; break;
				case 'u': help(); return 0; break;
			}
			++i;
		}
		
		//erros less than 3 arguments
		if(argc != 4)
		{
			printf("%s: bad syntax\n", argv[0]);
			return -2;
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
		msg_fd = open(argv[2], O_RDWR, 0);
		key_fd = open(argv[3], O_RDWR, 0);
		fstat(msg_fd, &msg_st);
		fstat(key_fd, &key_st);

		char* msg = (char*)malloc(sizeof(char) * (msg_st.st_size + 1));
		char* key = (char*)malloc(sizeof(char) * (key_st.st_size + 1));
		char *out;

		//performing text file
		if(msg_fd == NULL)
		{
			printf("%s: can't open file - %s\n", argv[0], argv[2]);
			return -1;
		}
		
		read(msg_fd, msg, msg_st.st_size * sizeof(char));		
		msg[msg_st.st_size] = '\0';
		//performing key

		
		if(key_fd == NULL)
		{
			printf("%s: can't open file - %s\n", argv[0], argv[3]);
			return -1;
		}

		read(key_fd, key, key_st.st_size * sizeof(char));
		key[key_st.st_size] = '\0';
		

		//calling crypt fun
		out = crypt(msg, key);
		
		//writing coded text
		lseek(msg_fd, 0L, 0);
		write(msg_fd, out, msg_st.st_size);
		
		close(msg_fd);
		close(key_fd);

		free(msg);
		free(key);
		free(out);

	}	//end coding
	else if(mode_d)		//decoding mode
	{	

		msg_fd = open(argv[2], O_RDWR, 0);
		key_fd = open(argv[3], O_RDWR, 0);
		fstat(msg_fd, &msg_st);
		fstat(key_fd, &key_st);

		char* msg = (char*)malloc(msg_st.st_size + sizeof(char));
		char* key = (char*)malloc(key_st.st_size + 1);
		char *out;
	

		//performing binary file
		if(msg_fd == NULL)
		{
			printf("%s: can't open file - %s\n", argv[0], argv[2]);
			return -1;
		}
		
		read(msg_fd, msg, msg_st.st_size);
		msg[msg_st.st_size] = '\0';
	
	//performing key
		if(key_fd == NULL)
		{
			printf("%s: can't open file - %s\n", argv[0], argv[3]);
			return -1;
		}

		read(key_fd, key, key_st.st_size);
		key[key_st.st_size] = '\0';


		//decrypting
		out = dcrypt(msg, key);

		//writing decoded text
		lseek(msg_fd, 0L, 0);
		write(msg_fd, out, msg_st.st_size);
		
		close(msg_fd);
		close(key_fd);
		
		free(msg);
		free(key);
		free(out);
	}	//end decode
	else		//if unknown mode set
	{
		printf("%s: bad syntax\n", argv[0]);
		return -2;
	}

	return 0;
}
