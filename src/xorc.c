#include <stddef.h>

//xcrypting function
int xcrypt(const char* data, const size_t data_size, 
const char* key, const size_t key_size,
char* result)
{
	unsigned int cur_jmp = 2;
	unsigned int indx = 0;
	
	//some magic
	for(size_t i = 0; i < data_size; ++i)
	{
		result[i] = data[i];
		
		indx = i & 1;
		do
		{
			result[i] ^= key[indx];
			
			indx = (indx + cur_jmp) % key_size;
		} while(indx != (i & 1));

		if(i & 1)
			++cur_jmp;
	}
	
	return 0;
}

