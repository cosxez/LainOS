ENTRY(_start)
SECTIONS {
	. = 1M;
	.boot : {KEEP(*(.multiboot_header))}
	.text : {*(.text)}
	.rodata : {*(.rodata)}
	.data : {*(.data)}
	.bss :
	{
		. = ALIGN(4096);
		*(COMMON)
		*(.bss)
	}
}
