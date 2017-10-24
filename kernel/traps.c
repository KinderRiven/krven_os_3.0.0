#include <system.h>
#include <unistd.h>
//DE-0
void divide_error();
//TS-10
void tss_error();
//GP
void protect_error();
//PF-14
void page_error();

void do_error(uint32_t err_type, uint32_t err_code)
{
	printc(c_black, c_red, "[ERROR %d]error code %d\n", err_type, err_code);
	while(1);		
}

int trap_init() 
{
	set_trap_gate( 0, (uint32_t)divide_error);	
	set_trap_gate(10, (uint32_t)tss_error);
	set_trap_gate(13, (uint32_t)protect_error);
	set_trap_gate(14, (uint32_t)page_error);
	return 1;
}
