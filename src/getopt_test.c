#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char** argv)
{
	char opt;
	char* infile, *outfile;
	bool input, output;
	infile = outfile = NULL;
	input = output = false;

	while ((opt = getopt(argc, argv, "-io")) != -1)
	{
		switch (opt)
		{
			case 'i':
				input = true;
			break;

			case 'o':
				output = true;
			break;
			
			case 1:
				if (infile)
				{
					outfile = optarg;
					printf("outfile: %s\n", optarg);
				}
				else
				{
					infile = optarg;
					printf("infile: %s\n", optarg);
				}
			break;

			default:
				return 1;
		}
	}

	if (input)
		printf("input\n");
	if (output)
		printf("output\n");
	
	return 0;
}
