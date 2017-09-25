#include <asm.h>

inline void sti() {
	asm volatile("sti");
}

inline void cli() {
	asm volatile("cli");
}
