/*
	This file is part of XORCrypter.

    XORCrypter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with XORCrypter.  If not, see <https://www.gnu.org/licenses/>.
*/

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

