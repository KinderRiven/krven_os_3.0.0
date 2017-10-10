#include <types.h>
#include <memory.h>

static uint32_t *mem_dir;
static uint32_t *mem_page[NR_PAGES];
static uint32_t  mem_stack[NR_MM_PAGES];
static int mem_stack_top;

static void page_init()
{
	int i, j;
	//link memory page ptr
	mem_dir = (uint32_t *) 0x0000;
	for(i = 0; i < NR_PAGES; i++) {
		mem_page[i] = (uint32_t *)((i + 1) * 0x1000);
	}
	//init memory directory
	uint32_t *tmp = mem_dir;
	for(i = 0; i < NR_PAGES; i++) {
		*tmp++ = ((uint32_t)mem_page[i] & PAGE_MASK) | PDE_RW | PDE_P | PDE_US;		
	}	
	for(i = NR_PAGES; i < PTE_COUNT; i++) {
		*tmp++ = 0;
	}
	//init memory page
	uint32_t addr = 0;
	for(i = 0; i < NR_PAGES; i++) {
		tmp = mem_page[i];
		for(j = 0; j < PTE_COUNT; j++) {
			*tmp++ = (uint32_t)(addr  & PAGE_MASK)| PTE_RW | PTE_P | PTE_US;	
			addr += PAGE_SIZE;
		}
	}
	//update cr0 cr3 to open paging
	asm volatile("mov %0, %%cr3"::"r"(mem_dir));	
	uint32_t cr0;
	asm volatile("mov %%cr0, %0":"=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0"::"r"(cr0));
}

static void stack_init(uint32_t memory_start, uint32_t memory_end) 
{
	uint32_t addr;
	memory_start &= PAGE_MASK;
	memory_end &= PAGE_MASK;
	mem_stack_top = 0;
	for(addr = memory_start; addr < memory_end; addr += PAGE_SIZE) {
		mem_stack[mem_stack_top++] = addr;
	}
}

int mem_init(uint32_t memory_start, uint32_t memory_end)
{
	page_init();
	stack_init(memory_start, memory_end);
	return 1;
}
