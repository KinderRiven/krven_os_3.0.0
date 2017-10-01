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

void kernel_main() {
	
	set_seg_descriptor(&gdt[1], 0, 0xFFFFFFFF, 0, D_RE);
	set_seg_descriptor(&gdt[2], 0, 0xFFFFFFFF, 0, D_RW);

	//init console
	if(con_init()) {
		//printc(c_black, c_green, "[OK] init console!\n");
	}
	
	//memory init
	memory_start = (1<<20);
	memory_start &= 0xfffff000;
	memory_end = (1<<20) + (EXT_MEM_K<<10);
	memory_end &= 0xfffff000;
	printc(c_black, c_light_red, "[OK] mem_start:%dMB,mem_end:%dMB\n", 
		memory_start/(1024*1024), memory_end/(1024*1024));
	if(mem_init(memory_start, memory_end)) {
		//printc(c_black, c_green, "[OK] init memory!\n");
	}
	
	//set trap gate
	if(trap_init()) {
		//printc(c_black, c_green, "[OK] init trap!\n");
	}

	//init sched [set timer, task0]
	if(sched_init()) {
		//printc(c_black, c_green, "[OK] init sched!\n");
	}

	//sti
	sti();
	move_to_user_mode();
	for(;;);
}
