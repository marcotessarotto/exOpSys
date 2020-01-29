#ifndef FILES_COMMON_H_
#define FILES_COMMON_H_

void test_mmap(void);

void * create_anon_mmap(size_t size);

void print_system_info(void);

unsigned long get_freeram();

#endif /* FILES_COMMON_H_ */
