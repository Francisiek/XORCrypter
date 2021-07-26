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

/*
Xcrypt function takes 'data' and performs xor operations
on it with 'key'. 'result' is a pointer to memory to which
result will be saved and that memory should have size of 
the 'data' so as 'data_size'.
*/
int xcrypt(const char* data, const size_t data_size, 
const char* key, const size_t key_size,
char* result);
