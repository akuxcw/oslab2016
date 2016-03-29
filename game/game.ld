OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(game_main)

SECTIONS
{
/*	. = 0x2000000;*/
	. = 0x8000000;

	.text : {
		*(.text .stub .text.* .gnu.linkonce.t.*)
	}

}
SECTIONS
{
	/*. = 0x2000000;*/

	. = ALIGN(0x1000);
/*	. += 0x2000;*/
	.rodata : {
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}

	.stab : {
		PROVIDE(__STAB_BEGIN__ = .);
		*(.stab);
		PROVIDE(__STAB_END__ = .);
		BYTE(0)		/* Force the linker to allocate space
				   for this section */
	}

	.stabstr : {
		PROVIDE(__STABSTR_BEGIN__ = .);
		*(.stabstr);
		PROVIDE(__STABSTR_END__ = .);
		BYTE(0)		/* Force the linker to allocate space
				   for this section */
	}

	. = ALIGN(0x1000);

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
	}

	/DISCARD/ : {
		*(.eh_frame .note.GNU-stack)
	}
}
