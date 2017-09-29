#include <sched.h>
#include <asm.h>
#include <system.h>
#include <io.h>
#include <memory.h>

extern void timer_interrupt();
extern uint32_t page_dir;

union task_union {
	struct task_struct task;
	char stack[PAGE_SIZE];
};

char user_stack[PAGE_SIZE];
static union task_union init_task;
struct task_struct *task[NR_TASKS] = {&(init_task.task),};

static void set_timer(int frequency) 
{
	uint32_t divisor = 1193180 / frequency;
	outb(0x43, 0x36);
	uint8_t low = (uint8_t) (divisor & 0xFF);
	uint8_t high = ((uint8_t)divisor >> 8) & 0xFF;
	outb(0x40, low);
	outb(0x40, high);
	outb(0x21, inb(0x21)&~0x01);
}

static void set_init_task() 
{	
	//set ldt segment [base limit dpl type]
	set_seg_descriptor(&init_task.task.ldt[1], 0, 0xFFFFFFFF, 3, D_RE);
	set_seg_descriptor(&init_task.task.ldt[2], 0, 0xFFFFFFFF, 3, D_RW);
	
	//set ldt date	
	set_ldt_descriptor(0, (uint32_t)init_task.task.ldt, 3);	
	
	//set tss gate
	set_tss_descriptor(0, (uint32_t)&init_task.task.tss);
	init_task.task.tss.esp0 = (uint32_t)&init_task.task + PAGE_SIZE;
	init_task.task.tss.ss0 = 0x10;	
	init_task.task.tss.esp2 = (uint32_t)user_stack + PAGE_SIZE;
	init_task.task.tss.ss2 = 0x17;
	init_task.task.tss.cr3 = (uint32_t)&page_dir;
	init_task.task.tss.trace_bitmap = 0x80000000;
	init_task.task.tss.eflags = 0;
	init_task.task.tss.cs = init_task.task.tss.es = init_task.task.tss.fs = 0x17;
	init_task.task.tss.ds = init_task.task.tss.ss = init_task.task.tss.gs = 0x17;
	__asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
	
	//set ldt register
	lldt(0);
	//set tss register
	ltr(0);
}

int sched_init() 
{
	set_intr_gate(0x20, (uint32_t)timer_interrupt);
	set_init_task();
	set_timer(100);
	return 1;
}
