#define VERSION 1.0f
#define AUTHOR "Francisiek"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
	int keyr=1;		//if key was in arguments, do not require on input
	if(argc == 3)
		keyr=0;

	if(!strcmp(argv[1], "-d"))	//decode msg
	{
		system("clear");
		//input msg and key
		int N=0;
		scanf("%d", &N);
		
		int val[N];
		for(int i=0; i<N; ++i)
		{
			scanf("%d", val+i);		//ascii values
		}
		
		char key[50];
		if(keyr)
			scanf(" %s", key);	//if key wasn't in arg
		system("clear");	//clear screen of key

		//do crypto
		char* p;	//output msg pointer
		if(keyr)
			p=dcrypt(val, N, key);	//if key was in input
		else 
			p=dcrypt(val, N, argv[2]);	//or arguments

		printf("%s\n", p);			//show output
		
		system("sleep 5");
		system("clear");	//wait and clear screen

	}
	else if(!strcmp(argv[1], "-c"))	//code msg
	{
		system("clear");
		char msg[50];
		char key[50];
        int* p;	//int table pointer

		scanf("%s", msg);	//take msg
		if(keyr)
			scanf(" %s", key);	//if key wasn't in arguments
		system("clear");
        
        int N=strlen(msg);

		//do crypto
		if(keyr)
			p=crypt(msg, key);		//key req
		else
			p=crypt(msg, argv[2]);	//key in argument

		//output result
		printf("%d\n", N);	//number of characters
		while(*p != 0)
			printf("%d\n", *p++);	//ascii values

	}
	else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "-u"))	//output help
		help();
	else if(!strcmp(argv[1], "-v"))		//output usage
		version();
	
	return 0;
}
