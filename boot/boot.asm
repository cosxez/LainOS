MBOOT_PAGE_ALIGN equ 1 << 0
MBOOT_MEM_INFO equ 1 << 1
MBOOT_VIDEO_MODE equ 1 << 2
MBOOT_HEADER_MAGIC equ 0x1BADB002
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_VIDEO_MODE
MBOOT_CHECKSUM equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .multiboot
align 4
	dd 0x1BADB002
	dd 0x00000007
	dd -(0x1BADB002 + 0x00000007)
	
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0

	dd 0
	dd 1024
	dd 768
	dd 32

section .bss
align 16
stack_bootom:
	resb 16384
stack_top:

section .text
global _start
extern kernel_main
global read_kbd_scan
extern keyboard_read

read_kbd_scan:
	pushad
	cld
	call keyboard_read
	popad
	iretd

_start:
	mov esp,stack_top
	
	extern gdt_table
	call gdt_table

	jmp 0x08:.reload_cs

.reload_cs:
	mov ax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax

	extern idt_table
	call idt_table

	push ebx
	sti
	call kernel_main

.hang:	
	hlt
	jmp .hang
