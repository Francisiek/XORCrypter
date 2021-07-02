#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[])
{
	if(argc > 1)
		srand(atoi(argv[1]));
	else
		srand(time(NULL));

	int Len;
	Len = rand() % 100 + 1;
	
	unsigned char ran_text[Len + 1];
	for(int i=0; i<Len; i++)
		ran_text[i] = (unsigned)rand() % 94 + 33;
	ran_text[Len] = '\0';

	printf("%s\n", ran_text);
	return 0;
}
