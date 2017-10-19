#include "types.h"
#include "system.h"

extern int system_call(void);

int system_call_init() 
{
	set_system_gate(0x80, (uint32_t)system_call);
	return 1;
}
