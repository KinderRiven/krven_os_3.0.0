#include <types.h>
#include <system.h>
#include <sched.h>

extern void con_init();
extern void trap_init();

void kernel_main() {
	trap_init();
	con_init();
	sched_init();
}
