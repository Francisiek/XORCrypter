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
Adds 'ptr' do pointers table.
Returns 0 on success and -1
if it couldn't find space for new
pointer.
*/
int alloc_mem_add(void* ptr);

/*
Deletes 'ptr' from pointers table.
On success returns 0.
If ptr doesn't exist in the table
returns -1.
*/
int alloc_mem_del(void* ptr);

/*
Frees memory to which pointers points.
*/
void free_mem(void);
