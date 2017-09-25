#include <sched.h>
#include <system.h>
#include <io.h>

extern void timer_interrupt();

struct task_t *task[NR_TASKS];

static void set_timer(int frequency) {
	uint32_t divisor = 1193180 / frequency;
	outb(0x43, 0x36);
	uint8_t low = (uint8_t) (divisor & 0xFF);
	uint8_t high = ((uint8_t)divisor >> 8) & 0xFF;
	outb(0x40, low);
	outb(0x40, high);
	outb(0x21, inb(0x21)&~0x01);
}

static void set_init_task() {
	
}

void sched_init() {
	set_intr_gate(0x20, (uint32_t)timer_interrupt);
	set_timer(100);
}
