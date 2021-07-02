#include <stdlib.h>

#define ALLOC_PTR 5

void* alloc_mem[ALLOC_PTR];

int alloc_mem_add(void* ptr)
{
	for(int i = 0; i < ALLOC_PTR; ++i)
	{
		if(alloc_mem[i] == ptr)
		{
			return 0;
		}
		else if(!alloc_mem[i])
		{
			alloc_mem[i] = ptr;
			return 0;
		}
	}

	return -1;
}

int alloc_mem_del(void* ptr)
{
	for(int i = 0; i < ALLOC_PTR; ++i)
	{
		if(alloc_mem[i] == ptr)
		{
			alloc_mem[i] = NULL;
			return 0;
		}
	}

	return -1;
}

void free_mem(void)
{
	for(int i = 0; i < ALLOC_PTR; i++)
	{
		if(alloc_mem[i])
		{
			free(alloc_mem[i]);
		}
	}
}

