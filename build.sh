nasm -f elf32 boot/boot.asm -o boot.o

gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gcc -m32 -T linker.ld -o output/LainOS.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc -no-pie -z noexecstack -Wl,--no-warn-rwx-segments

rm boot.o kernel.o
