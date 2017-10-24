#include <system.h>
#include <unistd.h>
extern int current_index;
//DE-0
void divide_error();
//PF-14
void page_error();

void do_divide_error() 
{

}

void do_page_error() 
{
	printk("Now current %d\n", current_index);
}

int trap_init() 
{
	set_trap_gate( 0, (uint32_t) divide_error);	
	set_trap_gate(14, (uint32_t) page_error);
	return 1;
}
