gcc -m64 -static src/init.c binaries/font.o binaries/cvnt.o -o src/output/init -Iinclude/
chmod +x src/output/init
cd src/output/
find . | cpio -o -H newc | gzip > ../../oiso/src/rinit
cd .. && cd ..
grub-mkrescue -o LainOS.iso oiso/
