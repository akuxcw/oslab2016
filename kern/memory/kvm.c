#include <common.h>
#include <inc/mmu.h>
#include <inc/x86.h>
#include <inc/string.h>
/* One TSS will be enough for all processes in ring 3. It will be used in Lab3. */
static TSS tss; 


inline static void
set_tss(SegDesc *ptr) {
	tss.ss0 = SELECTOR_KERNEL(SEG_KERNEL_DATA);		// only one ring 0 stack segment
//	tss.esp0 = 0x8000;
	uint32_t base = (uint32_t)&tss;
	uint32_t limit = sizeof(TSS) - 1;
	ptr->limit_15_0  = limit & 0xffff;
	ptr->base_15_0   = base & 0xffff;
	ptr->base_23_16  = (base >> 16) & 0xff;
	ptr->type = STS_T32A;
	ptr->segment_type = 0;
	ptr->privilege_level = DPL_USER;
	ptr->present = 1;
	ptr->limit_19_16 = limit >> 16;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 0;
	ptr->base_31_24  = base >> 24;
}

void
set_tss_esp0(int esp) {
	tss.esp0 = esp;
}

/* GDT in the kernel's memory, whose virtual memory is greater than 0xC0000000. */
static SegDesc gdt[NR_SEGMENTS];

void
set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit) {
	ptr->limit_15_0  = limit & 0xFFFF;
	ptr->base_15_0   = base & 0xFFFF;
	ptr->base_23_16  = (base >> 16) & 0xFF;
	ptr->type = type;
	ptr->segment_type = 1;
	ptr->privilege_level = pl;
	ptr->present = 1;
	ptr->limit_19_16 = (limit >> 16 ) & 0xF;
	ptr->soft_use = 0;
	ptr->operation_size = 0;
	ptr->pad0 = 1;
	ptr->granularity = 1;
	ptr->base_31_24  = (base >> 24) & 0xFF;
}

void
init_segment(void) {
	memset(gdt, 0, sizeof(gdt));
	set_segment(&gdt[SEG_KERNEL_CODE], DPL_KERNEL, SEG_EXECUTABLE | SEG_READABLE, 0, 0xFFFFF);
	set_segment(&gdt[SEG_KERNEL_DATA], DPL_KERNEL, SEG_WRITABLE, 0, 0xFFFFF);
	set_segment(&gdt[SEG_USER_CODE], DPL_USER, SEG_EXECUTABLE | SEG_READABLE, SEG_OFFSET, 0x1fff);
	set_segment(&gdt[SEG_USER_DATA], DPL_USER, SEG_WRITABLE, 2*SEG_OFFSET, 0x1fff);

	write_gdtr(gdt, sizeof(gdt));

	set_tss(&gdt[SEG_TSS]);
	ltr(SELECTOR_USER(SEG_TSS));
}

void init_memory() {
	
}

uint32_t mm_malloc(uint32_t vaddr, uint32_t size) {
#ifdef USE_PAGE
#else

#endif
	return 0;
}
