#ifndef __IRQ_H__
#define __IRQ_H__

#include <inc/types.h>

typedef struct {
	uint32_t edi, esi, ebp, old_esp, ebx, edx, ecx, eax;
	uint32_t gs, fs, es, ds;
	int32_t irq;
	uint32_t error_code;
	uint32_t eip, cs, eflags;
	uint32_t esp, ss;
} TrapFrame_iret;

typedef struct TrapFrame {
	uint32_t edi, esi, ebp, old_esp, ebx, edx, ecx, eax;
	int32_t irq;
	uint32_t error_code;
} TrapFrame;

#endif
