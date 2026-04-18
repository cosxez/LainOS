nasm -f elf64 boot/boot.asm -o boot.o

gcc -m64 -c kernel.c -o kernel.o -Iinclude -std=gnu99 -mgeneral-regs-only -ffreestanding -O2 -Wall -Wextra \
    -fno-stack-protector -mno-sse -fno-pie -mcmodel=kernel

gcc -m64 -T linker.ld -o output/LainOS.bin -ffreestanding -O2 -nostdlib \
    boot.o kernel.o cvnt.o font.o -lgcc -no-pie -z noexecstack -Wl,--no-warn-rwx-segments

rm boot.o kernel.o

cp output/LainOS.bin oiso/boot
grub-mkrescue -o LainOS.iso oiso
