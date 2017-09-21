#include <system.h>
#include <types.h>
//byte 0, 1 -> offset (00 - 15)
//byte 2, 3 -> selector
//byte 4, 5 -> nature
//byte 6, 7 -> offset (16 - 31)

//nature
//--------------------------
//|P|DPL|S|TYPE|0|0|0|     |
//--------------------------
//|1| 2 |1| 4  |1|1|1|  5  |
//--------------------------

static void set_gate(descriptor_t *gate_addr, uint8_t type, uint8_t dpl, uint32_t addr) {
	uint16_t *ptr = (uint16_t *) gate_addr;
	*ptr++ = (addr & 0xFFFF);
	*ptr++ = 0x0008;
	*ptr++ = (((uint16_t)dpl << 13) | ((uint16_t)type << 8) | 0x8000);
	*ptr++ = ((addr >> 16) & 0xFFFF);
}

void set_intr_gate(uint8_t n, uint32_t addr) {
	set_gate(&idt[n], 14, 0, addr);
}

void set_trap_gate(uint8_t n, uint32_t addr) {
	set_gate(&idt[n], 15, 0, addr);
}
