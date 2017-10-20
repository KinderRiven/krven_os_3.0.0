#include "types.h"
#include "system.h"
#include "sys.h"
#include "unistd.h"

extern int system_call();

fn_ptr sys_call_table[__NR_SYS_CALL];

int system_call_init() 
{
	set_system_gate(0x80, (uint32_t)system_call);
	sys_call_table[__NR_test] = sys_test;
	sys_call_table[__NR_fork] = sys_fork;
	return 1;
}
