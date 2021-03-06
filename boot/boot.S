#include <inc/mmu.h>
.code16
.globl start
start:
	cli
    # Graph mode
//	movw    $0x13, %ax
//	int     $0x10
	
	# Real color mode
	movw    $0x4f02, %ax
	movw	$0x0115, %bx
	int     $0x10
	
	xorw    %ax, %ax
	movw    %ax, %ds
	movw    %ax, %es
	movw    %ax, %ss

seta20.1:
  	inb     $0x64,%al               # Wait for not busy
    testb   $0x2,%al
	jnz     seta20.1

	movb    $0xd1,%al               # 0xd1 -> port 0x64
	outb    %al,$0x64

seta20.2:
	inb     $0x64,%al               # Wait for not busy
	testb   $0x2,%al
	jnz     seta20.2

	movb    $0xdf,%al               # 0xdf -> port 0x60
	outb    %al,$0x60

	movw	$0x4f01, %ax
	movw	$0x115, %cx
	movw	$0xa01, %di
	int 	$0x10

	lgdt    gdtdesc
	movl    %cr0, %eax
	orl     $0x1, %eax
	movl    %eax, %cr0

	ljmp    $GDT_ENTRY(1), $start32

.code32
start32:
	movw    $GDT_ENTRY(2), %ax
	movw    %ax, %ds
	movw    %ax, %es
	movw    %ax, %ss

	movl    $0x8000000, %esp
	call    bootmain

.p2align 2
gdt:
	SEG_NULL
	SEG(0xA, 0x0, 0xffffffff)
	SEG(0x2, 0x0, 0xffffffff)

gdtdesc:
	.word   (gdtdesc - gdt - 1)
	.long   gdt

