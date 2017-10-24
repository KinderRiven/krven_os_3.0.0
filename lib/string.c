#include <string.h>

inline int strlen(char *buf)
{
	int i;
	for(i = 0; buf[i]; i++);
	return i;
}

inline void memcpy(char *copy_from, char *copy_to, int copy_size)
{
	char *from_ptr = copy_from;
	char *to_ptr = copy_to;
	while(copy_size--) {
		*to_ptr = *from_ptr;
		to_ptr++;
		from_ptr++;
	}
} 
