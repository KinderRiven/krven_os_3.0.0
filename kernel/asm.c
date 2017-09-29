#include <asm.h>
#include <system.h>

inline void sti() 
{
	asm volatile("sti");
}

inline void cli() 
{
	asm volatile("cli");
}

inline void lldt(uint8_t n) 
{
	asm volatile("lldt %%ax"::"a"(_LDT(n)));	
}

inline void ltr(uint8_t n) 
{
	asm volatile("ltr %%ax"::"a"(_TSS(n)));
}

