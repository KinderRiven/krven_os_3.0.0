#include <system.h>
#include <types.h>
//nature
//--------------------------
//|P|DPL|S|TYPE|0|0|0|     |
//--------------------------
//|1| 2 |1| 4  |1|1|1|  5  |
//--------------------------


// 0. null
// 8. kernel text
//16. kernel data
//24. tss0
//30. ldt0

//gate descriptor
//byte 0, 1 -> offset (00 - 15)
//byte 2, 3 -> selector
//byte 4, 5 -> nature
//byte 6, 7 -> offset (16 - 31)
static void set_gate(descriptor_t *gate_addr, uint32_t offset, uint32_t selector, uint32_t nature) 
{	
	uint16_t *ptr = (uint16_t *) gate_addr;
	//0 1
	*ptr++ = (uint16_t)(offset & 0xFFFF);
	//2 3
	*ptr++ = (uint16_t)(selector & 0xFFFF);
	//4 5
	*ptr++ = (uint16_t)(nature & 0xFFFF);
	//6 7
	*ptr++ = (uint16_t)((offset >> 16) & 0xFFFF);
}

//segment descriptor
//byte 0, 1    -> limit (low)
//byte 2, 3, 4 -> base address (low)
//byte 5, 6	   -> nature
//byte 7       -> base address (high)
static void set_descriptor(descriptor_t *seg_addr, uint32_t base, uint32_t limit, uint32_t nature) 
{
	uint8_t *ptr = (uint8_t*) seg_addr;
	//byte 0, 1
	*ptr++ = (uint8_t)(limit & 0xFF);
	*ptr++ = ((uint8_t)(limit >> 8) & 0xFF);
	//byte 2, 3, 4
	*ptr++ = (uint8_t)(base & 0xFF);
	*ptr++ = (uint8_t)((base >> 8) & 0xFF);
	*ptr++ = (uint8_t)((base >> 16) & 0xFF);
	//byte 5, 6
	*ptr++ = (uint8_t)(nature & 0xFF);
	*ptr++ = (uint8_t)((nature >> 8) & 0xFF);
	//byte 7
	*ptr++ = (uint8_t)((base >> 24) & 0xFF);
}

void set_seg_descriptor(descriptor_t *seg_addr, uint32_t base, uint32_t limit, uint32_t dpl, uint32_t type) 
{
	uint32_t nature = (uint32_t)type | (dpl << 5) | (((limit >> 16) & 0x0F) << 8) | 0xC000;
	set_descriptor(seg_addr, base, limit, nature);
}

//interrupt E
void set_intr_gate(uint8_t n, uint32_t addr) 
{
	set_gate(&idt[n], addr, 0x0008, (uint32_t)G_INT << 8);
}

//trap F
//dpl = 0
void set_trap_gate(uint8_t n, uint32_t addr) 
{
	set_gate(&idt[n], addr, 0x0008, (uint32_t)G_TRAP << 8);	
}

//dpl = 3
void set_system_gate(uint8_t n, uint32_t addr)
{
	set_gate(&idt[n], addr, 0x0008, (uint32_t)(G_TRAP|(3<<5)) << 8);			
}

//tss
void set_tss_descriptor(uint8_t n, uint32_t addr) 
{
	uint32_t limit = 103;
	uint32_t nature = (uint32_t)D_TSS | (((limit >> 16) & 0x0F) << 8) | 0x8000;
	set_descriptor(&gdt[2*n+FIRST_TSS_ENTRY], addr, limit, nature);
}

//ldt
void set_ldt_descriptor(uint8_t n, uint32_t addr, uint8_t num) 
{
	uint32_t limit = 8 * num - 1;
	uint32_t nature = (uint32_t)D_LDT | (((limit >> 16) & 0x0F) << 8) | 0x8000;
	set_descriptor(&gdt[2*n+FIRST_LDT_ENTRY], addr, limit, nature);
}
