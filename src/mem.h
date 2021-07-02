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
