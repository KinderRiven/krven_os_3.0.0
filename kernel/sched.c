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

char user_stack[NR_TASKS][PAGE_SIZE];

static union task_union init_task;
struct task_struct *task[NR_TASKS];
struct task_struct *current;
static int current_index = 0;

static void init_timer(uint32_t frequency) 
{
	set_intr_gate(0x20, (uint32_t)timer_interrupt);
	uint32_t divisor = 1193180 / frequency;
	outb(0x43, 0x36);
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
	outb(0x40, low);
	outb(0x40, high);
	outb(0x21, inb(0x21)&~0x01);
}

#ifndef _SWITCH_TO
#define switch_to(n) {\
struct {long a,b;} __tmp; \
__asm__("cmpl %%ecx,current\n\t" \
	"je 1f\n\t" \
	"movw %%dx,%1\n\t" \
	"xchgl %%ecx,current\n\t" \
	"ljmp *%0\n\t" \
	"jne 1f\n\t" \
	"clts\n" \
	"1:" \
	::"m" (*&__tmp.a),"m" (*&__tmp.b), \
	"d" (_TSS(n)),"c" ((long) task[n])); \
}	
#endif

void schedule()
{
	while(1) {
		current_index = ((current_index + 1) % NR_TASKS);
		if(task[current_index] != NULL) {
			break;
		}
	}
	switch_to(current_index);
}

void do_timer(uint16_t dpl)
{
	if(!dpl) {
		return;
	}
	schedule();
}

static void set_new_task(int num, union task_union *new_task, uint32_t enter_addr) 
{
	//add to point array
	task[num] = &(new_task->task);
	//set ldt segment [base limit dpl type]
	set_seg_descriptor(&(new_task->task.ldt[1]), 0, 0xFFFFFFFF, 3, D_RE);
	set_seg_descriptor(&(new_task->task.ldt[2]), 0, 0xFFFFFFFF, 3, D_RW);
	//set ldt date	
	set_ldt_descriptor(num, (uint32_t)new_task->task.ldt, 3);	
	//set tss gate
	set_tss_descriptor(num, (uint32_t)&(new_task->task.tss));
	//ss esp
	new_task->task.tss.esp0 = (uint32_t)&(new_task->task) + PAGE_SIZE;
	new_task->task.tss.ss0 = 0x10;	
	//cr3
	new_task->task.tss.cr3 = (uint32_t)&page_dir;
	//io
	new_task->task.tss.trace_bitmap = 0x80000000;
	//eflags
	new_task->task.tss.eflags = 0x202;
	//cs es fs ds ss gs
	new_task->task.tss.cs = 0x0f;
	new_task->task.tss.ss = new_task->task.tss.fs = new_task->task.tss.ds =  \
	new_task->task.tss.es = new_task->task.tss.gs = 0x17;
	//eip
	new_task->task.tss.eip = enter_addr;
	new_task->task.tss.esp = (uint32_t)user_stack[num] + PAGE_SIZE;
}

static void set_init_task() 
{	
	set_new_task(0, &init_task, 0);
	__asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
	//set ldt register
	lldt(0);
	//set tss register
	ltr(0);
	current = &init_task.task; 
}

int sched_init() 
{
	set_init_task();
	init_timer(100);
	return 1;
}
