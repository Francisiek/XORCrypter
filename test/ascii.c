#include <stdio.h>

int main(void)
{
	printf("DEC\tHEX\tCHAR\n");
	for(int i = 0; i < 129; i++)
	{
		printf("%d\t%x\t%c\n", i, i, i);
	}
	return 0;
}
