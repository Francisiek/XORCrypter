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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "mem.h"

void handle_err(int rt, char* format, ...)
{
	va_list args;
	va_start(args, format);

	while(*format)
	{
		if(strncmp(format, "%d", 2) == 0)
		{
			fprintf(stderr, "%d", va_arg(args, int));
			format += 2;
		}
		else if(strncmp(format, "%s", 2) == 0)
		{
			fprintf(stderr, "%s", va_arg(args, char*));
			format += 2;
		}
		else
		{
			fprintf(stderr, "%c", *format);
			format++;
		}
	}

	va_end(args);

	free_mem();
	exit(rt);
}


