#include <types.h>
#include <system.h>
#include <sched.h>
#include <asm.h>

extern void con_init();
extern void trap_init();
extern void mem_init();

void kernel_main() {
	
	mem_init();
	//set trap gate
	trap_init();
	
	//init console
	con_init();

	//init sched
	//set timer, new init task
	sched_init();

	//sti
	//sti();
}
