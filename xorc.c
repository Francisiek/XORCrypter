//Francisiek (C) 2020
#define VERSION 1.2f
#define AUTHOR "Francisiek"
#define LOG printf("log\n");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	
	char* out = malloc((N+1) * sizeof(char));
	
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
	
	out[N] = '\0';
	
	return out;
}

//decrypting func
char* dcrypt(const char* text, int N, const char* key)
{
	int M = strlen(key);
	char* out = malloc((N+1) * sizeof(char));
	
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
	
	out[N]='\0';

	return out;
}


int main(int argc, char* argv[])
{
	bool mode_c, mode_d;
	FILE *msgf, *keyf;
	
	//default values
	mode_c = mode_d = FALSE;
	msgf = keyf = NULL;

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
		char msg[BUFSIZ];
		char key[BUFSIZ];
		char *out, *tmp;

		//performing text file
		{
			char c;
			int it=0;
			
			msgf = fopen(argv[2], "r");
			
			if(msgf == NULL)
			{
				printf("%s: can't open file - %s\n", argv[0], argv[2]);
				return -1;
			}

			while((c = getc(msgf)) != EOF && (it != BUFSIZ))
				msg[it++] = c;	
			
			msg[it] = '\0';

			fclose(msgf);
		}
		//performing key
		{
			char c;
			int it=0;
			
			keyf = fopen(argv[3], "r");
			
			if(keyf == NULL)
			{
				printf("%s: can't open file - %s\n", argv[0], argv[3]);
				return -1;
			}

			while((c = getc(keyf)) != EOF && (it != BUFSIZ))
				key[it++] = c;
				
			key[it] = '\0';

			fclose(keyf);
		}
		
		//calling crypt fun
		out = crypt(msg, key);
		tmp = out;
		
		int N = strlen(msg);
		
		//writing coded text
			msgf = fopen(argv[2], "wb");
			
			if(msgf == NULL)
			{
				printf("%s: can't open file - %s\n", argv[0], argv[2]);
				return -1;
			}
			
			fwrite(&N, sizeof(int), 1, msgf);
						
			fwrite(out, sizeof(char), N, msgf);
			
			fclose(msgf);

		free(tmp);
	}	//end coding
	else if(mode_d)		//decoding mode
	{
		int N;
		char msg[BUFSIZ];
		char key[BUFSIZ];
		char *out, *tmp;

		//performing binary file
			msgf = fopen(argv[2], "rb");
			
			if(msgf == NULL)
			{
				printf("%s: can't open file - %s\n", argv[0], argv[2]);
				return -1;
			}
			
			fread(&N, sizeof(int), 1, msgf);
						
			fread(msg, sizeof(char), N, msgf);
			
			fclose(msgf);
		
		//performing key
			char c;
			int it=0;
			
			keyf = fopen(argv[3], "r");
			
			if(keyf == NULL)
			{
				printf("%s: can't open file - %s\n", argv[0], argv[3]);
				return -1;
			}
	
			while((c = getc(keyf)) != EOF && (it != BUFSIZ))
				key[it++] = c;
				
			key[it] = '\0';

			fclose(keyf);

		//decrypting
		out = dcrypt(msg, N, key);
		tmp = out;

		//writing decoded text
			msgf = fopen(argv[2], "w");
			
			if(msgf == NULL)
			{
				printf("%s: can't open file - %s\n", argv[0], argv[2]);
				return -1;
			}

			while(*out)
				putc(*out++, msgf);

			fclose(msgf);

		free(tmp);
	}	//end decode
	else		//if unknown mode set
	{
		printf("%s: bad syntax\n", argv[0]);
		return -2;
	}

	return 0;
}
