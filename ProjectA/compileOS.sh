dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
bcc kernel.c -ansi -c  -o kernel_c.o
as86 kernel.asm -o kernel_asm.o
ld86 -d kernel_c.o kernel_asm.o -o kernel
dd if=kernel of=floppy.img bs=512 conv=notrunc seek=3
# java -jar simulator.jar floppy.img
