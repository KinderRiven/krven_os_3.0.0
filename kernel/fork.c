#include "sched.h"
#include "system.h"
#include "memory.h"
#include "unistd.h"

extern struct task_struct *task[NR_TASKS];

int find_empty_process() 
{
	int i;
	for(i = 1; i < NR_TASKS; i++) {
		if(task[i] == NULL) {
			return i;	
		}
	}
	return -1;
}

int copy_process(int nr, uint32_t ebp, uint32_t edi, uint32_t esi, uint32_t gs, uint32_t none,
	uint32_t ebx, uint32_t ecx, uint32_t edx,
	uint32_t fs,  uint32_t es,  uint32_t ds,
	uint32_t eip, uint32_t cs,  uint32_t eflags ,uint32_t esp, uint32_t ss) 
{
	printk("nr[%d] ebp[0x%x] edi[0x%x]\nesi[0x%x] gs[0x%x] retaddr[0x%x]\nebx[0x%x] ecx[0x%x] edx[0x%x]\nfs[0x%x] es[0x%x] ds[0x%x]\neip[0x%x] cs[0x%x] eflags[0x%x] esp[0x%x] ss[0x%x]\n", nr, ebp, edi, esi, gs, none, ebx, ecx, edx, fs, es, ds, eip, cs, eflags, esp, ss);
	struct task_struct *p = (struct task_struct*) get_free_page();
	task[nr] = p;
	*p = *current;
	//update tss struct
	p->tss.back_link = 0;
	p->tss.esp0 = (uint32_t)p+PAGE_SIZE;
	p->tss.ss0 = 0x10;
	p->tss.ebp = ebp;
	p->tss.edi = edi;
	p->tss.esi = esi;
	p->tss.ebx = ebx;
	p->tss.ecx = ecx;
	p->tss.edx = edx;
	//This is why fork child process return 0
	p->tss.eax = 0;
	p->tss.eip = eip;
	p->tss.eflags = eflags;
	p->tss.esp = esp;
	p->tss.gs = gs & 0xffff;
	p->tss.cs = cs & 0xffff;
	p->tss.fs = fs & 0xffff;
	p->tss.es = es & 0xffff;
	p->tss.ds = ds & 0xffff;
	p->tss.ss = ss & 0xffff;
	p->tss.ldt = _LDT(nr);
	p->tss.trace_bitmap = 0x80000000;
	//set ldt descriptor	
	set_ldt_descriptor(nr, (uint32_t)p->ldt, 3);	
	//set tss descriptor
	set_tss_descriptor(nr, (uint32_t)&(p->tss));
	return nr;
}
