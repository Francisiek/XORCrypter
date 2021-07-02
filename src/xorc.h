/*
Xcrypt function takes 'data' and performs xor operations
on it with 'key'. 'result' is a pointer to memory to which
result will be saved and that memory should have size of 
the 'data' so as 'data_size'.
*/
int xcrypt(const char* data, const size_t data_size, 
const char* key, const size_t key_size,
char* result);
