MAGIC equ 0xe85250d6
FPL32 equ 0

section .multiboot_header
align 8
header_start:
	dd MAGIC
	dd FPL32
	dd header_end-header_start
	dd 0x100000000-(MAGIC+FPL32+(header_end-header_start))

	align 8
	dw 5
	dw 0
	dd 20
	dd 1600
	dd 900
	dd 32

	align 8
	dw 0
	dw 0
	dd 8
header_end:

section .bss
align 4096
fr_table:
	resb 4096
sr_table:
	resb 4096
tr_table:
	resb 16384

stack_bottom:
	resb 16384
stack_top:

section .text
bits 32

global _start

_start:
	mov esp,stack_top
	
	mov eax,sr_table
	or eax,0b11
	mov [fr_table],eax

	mov eax, tr_table
	or eax,0b11
	mov [sr_table],eax

	mov eax,tr_table+4096
	or eax,0b11
	mov [sr_table+8],eax

	mov eax, tr_table + 8192
	or eax, 0b11
	mov [sr_table + 16], eax
    
	mov eax, tr_table + 12288
	or eax, 0b11
	mov [sr_table + 24], eax

	mov ecx,0
.map_tr_table:
	mov eax,0x200000
	mul ecx
	or eax,0b10000011
	mov [tr_table+ecx*8],eax
	
	inc ecx
	cmp ecx,4096
	jne .map_tr_table

	mov eax,cr4
	or eax,1<<5
	mov cr4,eax

	mov ecx,0xc0000080
	rdmsr
	or eax,1<<8
	wrmsr

	mov eax,fr_table
	mov cr3,eax
	mov eax,cr0
	or eax,1<<31
	mov cr0,eax

	lgdt [gdt64.pointer]
	jmp gdt64.code_segment:long_mode_start

bits 64
long_mode_start:
	mov rsp,stack_top

	mov ax,0
	mov ss,ax
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	mov edi,ebx

	extern kernel_main
	call kernel_main
	hlt

section .rodata
gdt64:
	dq 0
.code_segment: equ $ - gdt64
	dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.pointer:
	dw $ - gdt64-1
	dq gdt64
