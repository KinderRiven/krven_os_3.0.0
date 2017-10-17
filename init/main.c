#include <types.h>
#include <system.h>
#include <sched.h>
#include <memory.h>
#include <asm.h>
#include <console.h>
#include <kernel.h>

extern void move_to_user_mode();
extern int trap_init();
static uint32_t memory_start;
static uint32_t memory_end;

#define EXT_MEM_K (*((uint16_t*)0x90002))

#ifndef _move_to_user_mode()
#define _move_to_user_mode() \
__asm__ ("movl %%esp,%%eax\n\t" \
	"pushl $0x17\n\t" \
	"pushl %%eax\n\t" \
	"pushfl\n\t" \
	"pushl $0x0f\n\t" \
	"pushl $1f\n\t" \
	"iret\n" \
	"1:\tmovl $0x17,%%eax\n\t" \
	"movw %%ax,%%ds\n\t" \
	"movw %%ax,%%es\n\t" \
	"movw %%ax,%%fs\n\t" \
	"movw %%ax,%%gs" \
	:::"ax")
#endif

void kernel_main() {
	int i, flags = 0;
	set_seg_descriptor(&gdt[1], 0, 0xFFFFFFFF, 0, D_RE);
	set_seg_descriptor(&gdt[2], 0, 0xFFFFFFFF, 0, D_RW);

	//init console
	flags = con_init();
	if(flags) {}
	
	//memory init
	memory_start = (1<<20);
	memory_start &= 0xfffff000;
	memory_end = (1<<20) + (EXT_MEM_K<<10);
	memory_end &= 0xfffff000;
	flags = mem_init(memory_start, memory_end); 
	if(flags) {}
	
	//set trap gate
	flags = trap_init();
	if(flags) {}

	//init sched [set timer, task0]
	flags = sched_init();
	if(flags) {}
	
	//sti
	sti();
	move_to_user_mode();
	for(;;) {}
}
