/*
This function prints message with %d, %s and %c formatting
on standard error stream, frees memory using 'free_mem()'
and exits with 'rt' value.
*/
void handle_err(int rt, char* format, ...);
