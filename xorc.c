//Francisiek (C) 2020
#define VERSION 1.1f
#define AUTHOR "Francisiek"
#define LOG printf("log\n");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {FALSE=0, TRUE=1} bool;

void help(void)
{
	printf("\tUsage: passwdmg [arg] [optional key]\n");
	printf("\t-c crypt the input. Program waits for text and key.\n");
	printf("\tthen outputs lenght of output and ascii codes\n");
	printf("\t-d decrypt the input. Program waits for number of arguments\n");
	printf("\tthen for each arguments. Argument must be ascii dec code, and then for key\n");
	printf("\t-h, -u shows this message\n");
	printf("\t-v show program info\n");
}

void version(void)
{
	printf("Version: %.2f\n", VERSION);
	printf("Author: %s\n", AUTHOR);
}

int* crypt(const char* text, const char* key)
{
	int M=strlen(key);
	int N=strlen(text);
	int* out = malloc( (N+1) * sizeof(int) );
	
	for(int i=0; i<N; ++i)
	{
		out[i]=text[i];
		for(int j=0; j<M; ++j)
			out[i]=out[i] ^ key[ i*2+5 % M ];
	}
	out[N]='\0';

	return out;
}

char* dcrypt(const int* text, int N, const char* key)
{
	int M=strlen(key);
	char* out = malloc( (N+1) * sizeof(char) );
	
	for(int i=0; i<N; ++i)
	{	
		out[i]=text[i];
		for(int j=0; j<M; ++j)
			out[i] = out[i] ^ key[ i*2+5 % M ];
	}
	out[N]='\0';

	return out;
}


int main(int argc, char* argv[])
{
	bool mode_c, mode_d, mode_k, ver, keynr, file;
	FILE *msgf, *keyf;
	
	mode_c = mode_d = mode_k = ver = keynr = file = FALSE;
	msgf = keyf = NULL;

	if(argc > 1)
	{
		int i = 0;
		while(argv[1][i] !='-')
			i++;
		
		while(argv[1][i])
		{
			switch(argv[1][i])
			{
				case 'c': mode_c = TRUE; break;
				case 'd': mode_d = TRUE; break;
				case 'v': ver = TRUE; break;
				case 'k': mode_k = TRUE; break;
			}
			++i;
		}

		if(argc > 2 && (mode_d || mode_c))
		{
			if((msgf = fopen(argv[2], "r")) == NULL)
			{
				printf("%s: can't open file - %s\n", argv[0], argv[2]);
				return -1;
			}
			else
				file = TRUE;

			if(argc > 3)
			{
				keynr = TRUE;
				keyf = fopen(argv[3], "r");
			}	//if keyf is NULL then key is argv[3]
		}
	}
	else
		help();


	if(mode_c)
	{
		char msg[BUFSIZ];
		char key[BUFSIZ];
		int* out;

		if(file)
		{
			char c;
			int it=0;
			while((c = getc(msgf)) != EOF && (it != BUFSIZ))
				msg[it++] = c;	
			msg[it]='\0';

		}
		else
			scanf("%s", msg);

		if(keynr)
		{
			if(keyf != NULL)
			{
				char c;
				int it=0;
				while((c = getc(keyf)) != EOF && (it != BUFSIZ))
					key[it++] = c;
				key[it]='\0';
			}
			else
				strcpy(key, argv[3]);
		}
		else
			scanf("%s", key);
		

		out = crypt(msg, key);

		int N = strlen(msg);
		
		if(file)
		{
			msgf = fopen(argv[2], "w");
			fprintf(msgf, "%d\n", N);
			while(*out)
				fprintf(msgf, "%d\n", *out++);
		}
		else
		{
			printf("%d\n", N);
			while(*out)
				printf("%d\n", *out++);
		}
	}
	else if(mode_d)
	{
		int N;
		int msg[BUFSIZ];
		char key[BUFSIZ];
		char *out;

		if(file)
		{
			char tmp[5];
			int x;
			int it=0;

			N = atoi(fgets(tmp, BUFSIZ, msgf));
			
			while(((fgets(tmp, BUFSIZ, msgf)) != NULL) && (it != BUFSIZ) && (it < N))
			{
				x = atoi(tmp);
				msg[it++] = x;
			}
			
		}
		else
		{
			scanf("%d", &N);
			int i = N;
			while(i--)
				scanf("%d", msg + i);
		}
		
		if(keynr)
		{
			if(keyf != NULL)
			{
				char c;
				int it=0;
				while((c = getc(keyf)) != EOF && (it != BUFSIZ))
					key[it++] = c;
				key[it]='\0';
			}
			else
				strcpy(key, argv[3]);
		}
		else
			scanf("%s", key);
		
		out = dcrypt(msg, N, key);
		
		if(file)
		{
			msgf = fopen(argv[2], "w");
			while(*out)
				putc(*out++, msgf);
		}
		else
			printf("%s", out);

	}
	else if(ver)
		version();
	else
		help();
	
	if(file)
		fclose(msgf);
	if(keyf != NULL)
		fclose(keyf);

	return 0;
}
