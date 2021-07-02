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


