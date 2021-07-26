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

