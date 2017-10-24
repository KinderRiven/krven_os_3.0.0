#include <types.h>
#include <system.h>
#include <sched.h>
#include <sys.h>
#include <unistd.h>
#include <memory.h>
#include <asm.h>
#include <kernel.h>

extern fn_ptr sys_call_table();
extern void move_to_user_mode();
extern int trap_init();
static uint32_t memory_start;
static uint32_t memory_end;

#define EXT_MEM_K (*((uint16_t*)0x90002))

_syscall0(int, test);
_syscall0(int, fork);

void kernel_main() {
	int flags = 0;
	set_seg_descriptor(&gdt[1], 0, 0xFFFFFFFF, 0, D_RE);
	set_seg_descriptor(&gdt[2], 0, 0xFFFFFFFF, 0, D_RW);

	//init console
	flags = con_init();
	if(flags) {
		printc(c_black, c_green, "[SUCCESS] CONSOLE INIT\n");
	}
	else {
		printc(c_black, c_red, "[ERROR] CONSOLE INIT\n");
	}
	
	//memory init
	memory_start = (1<<20);
	memory_start &= 0xfffff000;
	memory_end = (1<<20) + (EXT_MEM_K<<10);
	memory_end &= 0xfffff000;
	flags = mem_init(memory_start, memory_end);
	if(flags) {
		printc(c_black, c_green, "[SUCCESS] MEMORY INIT\n");
	}
	else {
		printc(c_black, c_red, "[ERROR] MEMORY INIT\n");
	}
	
	//set trap gate
	flags = trap_init();
	if(flags) {
		printc(c_black, c_green, "[SUCCESS] TRAPS INIT\n");
	}
	else {
		printc(c_black, c_red, "[ERROR] TRAPS INIT\n");
	}
	
	//init sched [set timer, task0]
	flags = sched_init();
	if(flags) {
		printc(c_black, c_green, "[SUCCESS] SCHEDULE INIT\n");
	}
	else {
		printc(c_black, c_red, "[ERROR] SCHEDULE INIT\n");
	}
	
	//init system call
	flags = system_call_init();
	if(flags) {
		printc(c_black, c_green, "[SUCCESS] SYSTEM CALL INIT\n");
	}
	else {
		printc(c_black, c_red, "[ERROR] SYSTEM CALL INIT\n");
	}	
	
	//sti
	sti();
	move_to_user_mode();

	//fork	
	if(!fork()) {
		while(1);
		printc(c_black, c_red, "[SUCCESS] CHILD FORK[%d]\n", flags);
		while(1);
	} 
	else {
		while(1);
		printc(c_black, c_green, "[SUCCESS] PARENT FORK[%d]\n", flags);
		while(1);
	}
	for(;;) {}
}
