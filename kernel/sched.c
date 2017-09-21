#include <sched.h>
#include <system.h>
#include <io.h>

extern void timer_interrupt();

void sched_init() {
	set_intr_gate(0x20, (uint32_t)timer_interrupt); 
}
