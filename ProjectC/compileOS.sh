#makes a blank disc
dd if=/dev/zero of=floppy.img bs=512 count=2880
#put the bootloader at section 1
dd if=bootload of=floppy.img bs=512 count=1 conv=notrunc

#the next tree line compile and link to make the kernel
bcc kernel.c -ansi -c  -o kernel_c.o
as86 kernel.asm -o kernel_asm.o
ld86 -d kernel_c.o kernel_asm.o -o kernel

#compile and link the shell
bcc shell.c -ansi -c  -o shell.o
as86 lib.asm -o libShell.o
ld86 -d shell.o libShell.o -o shell

#load the kernel at sector 3
dd if=kernel of=floppy.img bs=512 conv=notrunc seek=3
#load message at sector 30
#dd if=message.txt of=floppy.img bs=512 count=1 seek=30 conv=notrunc
dd if=map.img of=floppy.img bs=512 count=1 seek=1 conv=notrunc 
dd if=dir.img of=floppy.img bs=512 count=1 seek=2 conv=notrunc 
# java -jar simulator.jar floppy.img
gcc -o loadFile loadFile.c
./loadFile message.txt
./loadFile tstprg
./loadFile shell

