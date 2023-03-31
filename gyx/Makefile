#


# target : prerequisites 
#	command 
# $@ 代表 target
# $< 代表 prerequisites 的第1个名字 
# 


ENTRYPOINT=0x30400
ENTRYOFFSET=0x400

ASM=nasm
CC=gcc
LD=ld
ASMBFLAGS= -f bin -I Include/
ASMKFLAGS= -f elf -I Include/
CFLAGS= -c -I Include/
LDFLAGS= -s -Ttext $(ENTRYPOINT)       #notice continue expand

depend= Include/type.h Include/protect.h Include/const.h Include/global.h \
	 Include/proto.h Include/proc.h Include/keymap.h  \
         Include/tty.h Include/console.h Kernel/tosh.h

depend_for_asm= Include/sconst.inc

BOOT=Boot/boot.bin Boot/loader.bin
KERNEL=Kernel/kernel.bin
OBJS=Kernel/kernel.o Kernel/cstart.o Kernel/i8259.o Kernel/protect.o \
	 Lib/glib.o Kernel/main.o Lib/glib2.o Kernel/clock.o \
	Kernel/syscall.o Kernel/proc.o Kernel/set8253.o Kernel/set8042.o \
	Kernel/tty.o Kernel/meet_vga.o Kernel/console.o Kernel/toyou.o \
	Kernel/printf.o Kernel/vsprintf.o

.PHONY : build move

build : $(BOOT) $(KERNEL)



Boot/boot.bin : Boot/boot.asm Boot/LC.asm Boot/pm.asm
	$(ASM) $(ASMBFLAGS) -o $@ $<

Boot/boot.asm : Boot/boot.dguu
	cp $< $@


Boot/loader.bin : Boot/loader.asm Boot/LC.asm Boot/pm.asm
	$(ASM) $(ASMBFLAGS) -o $@ $<

Boot/loader.asm : Boot/loader.dguu
	cp $< $@





Kernel/kernel.bin : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

Kernel/kernel.o : Kernel/kernel.asm $(depend_for_asm)
	$(ASM) $(ASMKFLAGS) -o $@ $<

Kernel/kernel.asm : Kernel/kernel.dguu
	cp $< $@

Include/sconst.inc : Include/sconst.dguu
	cp $< $@

Kernel/cstart.o : Kernel/cstart.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/cstart.c : Kernel/cstart.dguu
	cp $< $@

Kernel/protect.o : Kernel/protect.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/protect.c : Kernel/protect.dguu
	cp $< $@

Kernel/i8259.o : Kernel/i8259.c  $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/i8259.c : Kernel/i8259.dguu
	cp $< $@

Kernel/main.o : Kernel/main.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/main.c : Kernel/main.dguu
	cp $< $@


Kernel/clock.o : Kernel/clock.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/clock.c : Kernel/clock.dguu
	cp $< $@

Kernel/proc.o : Kernel/proc.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/proc.c : Kernel/proc.dguu $(depend)
	cp $< $@

Kernel/set8253.o : Kernel/set8253.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/set8253.c : Kernel/set8253.dguu 
	cp $< $@

Kernel/set8042.o : Kernel/set8042.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/set8042.c : Kernel/set8042.dguu 
	cp $< $@

Kernel/tty.o : Kernel/tty.c $(depend)
	$(CC) $(CFLAGS) -o $@ $<

Kernel/tty.c : Kernel/tty.dguu
	cp $< $@

Kernel/meet_vga.o : Kernel/meet_vga.c 
	$(CC) $(CFLAGS) -o $@ $<

Kernel/meet_vga.c : Kernel/meet_vga.dguu
	cp $< $@

Kernel/console.o : Kernel/console.c
	$(CC) $(CFLAGS) -o $@ $<

Kernel/console.c : Kernel/console.dguu
	cp $< $@

Kernel/toyou.o : Kernel/toyou.c
	$(CC) $(CFLAGS) -o $@ $<

Kernel/toyou.c : Kernel/toyou.dguu
	cp $< $@

Kernel/printf.o : Kernel/printf.c
	$(CC) $(CFLAGS) -o $@ $<
Kernel/printf.c : Kernel/printf.dguu
	cp $< $@

Kernel/vsprintf.o : Kernel/vsprintf.c
	$(CC) $(CFLAGS) -o $@ $<
Kernel/vsprintf.c : Kernel/vsprintf.dguu
	cp $< $@



Kernel/syscall.o : Kernel/syscall.asm $(depend)
	$(ASM) $(ASMKFLAGS) -o $@ $<

Kernel/syscall.asm : Kernel/syscall.dguu $(depend)
	cp $< $@


Include/type.h : Include/type.dguu
	cp $< $@
Include/const.h : Include/const.dguu
	cp $< $@
Include/global.h : Include/global.dguu
	cp $< $@
Include/keymap.h : Include/keymap.dguu
	cp $< $@
Include/proc.h : Include/proc.dguu
	cp $< $@
Include/protect.h : Include/protect.dguu
	cp $< $@
Include/proto.h : Include/proto.dguu
	cp $< $@
Include/tty.h : Include/tty.dguu
	cp $< $@
Include/console.h : Include/console.dguu
	cp $< $@
Kernel/tosh.h : Kernel/tosh.dguu               
	cp $< $@





Lib/glib.o : Lib/glib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

Lib/glib.asm : Lib/glib.dguu
	cp $< $@

Lib/glib2.o : Lib/glib2.c
	$(CC) $(CFLAGS) -o $@ $<
Lib/glib2.c : Lib/glib2.dguu 
	cp $< $@




move :
	dd if=Boot/boot.bin of=g.img bs=512 count=1 conv=notrunc
	mount -o loop g.img /media/floppy
	cp Boot/loader.bin /media/floppy/
	cp Kernel/kernel.bin /media/floppy/
	sleep 2
	umount /media/floppy

#1037D516
#negiup
