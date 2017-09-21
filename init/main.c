#include <types.h>
#include <system.h>
#include <sched.h>

extern void trap_init();

void kernel_main() {
	trap_init();
	sched_init();	
}
