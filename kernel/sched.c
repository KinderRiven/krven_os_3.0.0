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
static union task_union user_task;
struct task_struct *task[NR_TASKS];

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

static void set_new_task(int num, union task_union *new_task) 
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
	new_task->task.tss.esp2 = (uint32_t)user_stack[num] + PAGE_SIZE;
	new_task->task.tss.ss2 = 0x17;
	//cr3
	new_task->task.tss.cr3 = (uint32_t)&page_dir;
	//io
	new_task->task.tss.trace_bitmap = 0x80000000;
	//eflags
	new_task->task.tss.eflags = 0;
	//cs es fs ds ss gs
	new_task->task.tss.cs = new_task->task.tss.es = new_task->task.tss.fs = 0x17;
	new_task->task.tss.ds = new_task->task.tss.ss = new_task->task.tss.gs = 0x17;
	
}

static void set_init_task() 
{	
	set_new_task(0, &init_task);
	__asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
	//set ldt register
	lldt(0);
	//set tss register
	ltr(0);
}

static void set_user_task(union task_union *user_task)
{
	int i;
	for(i = 0; i < NR_TASKS; i++) {
		if(task[i] == NULL) {
			set_new_task(i, user_task);
			break;
		}
	}	
}

int sched_init() 
{
	set_intr_gate(0x20, (uint32_t)timer_interrupt);
	set_init_task();
	set_user_task(&user_task);
	set_timer(100);
	return 1;
}
