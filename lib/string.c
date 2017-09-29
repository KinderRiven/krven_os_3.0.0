#include <string.h>

inline int strlen(char *buf) {
	int i;
	for(i = 0; buf[i]; i++);
	return i;
}
