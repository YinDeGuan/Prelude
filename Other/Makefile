#


N1		= loader
N2		= kernel
N3		= dglib
N4		= cstart

ASM  		= nasm
CC   		= gcc
LD   		= ld
CP		= cp
ASMFLAGS  	= -f elf
CFLAGS		= -c
LDFLAGS		= -s -Ttext 0x30400
CPFLAGS		= 

BIN		= logocoop
OBJS		= logo.o coop.o





BUILDNEW : $(N2).bin

$(N2).bin : $(N2).o $(N3).o $(N4).o
	$(LD) $(LDFLAGS) -o $@ $(N2).o $(N3).o $(N4).o 


$(N2).o : $(N2).asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(N2).asm : $(N2).dguu
	cp $< $@


$(N3).o : $(N3).asm
	$(ASM) $(ASMFLAGS) -o $@ $<

$(N3).asm : $(N3).dguu
	cp $< $@


$(N4).o : $(N4).c ./ptype.h
	$(CC) $(CFLAGS) -o $@ $<

$(N4).c : $(N4).dguu
	cp $< $@





#make会考虑将要执行的规则所需要的文件是否更新 ,若是没有,则什么都不做.
#


#BUILD : $(N1).bin $(N2).bin

#$(N1).bin : $(N1).asm
#	$(ASM) -f BIN -o $@ $<
#
#$(N1).asm : $(N1).dguu
#	cp $< $@





#$(N2).bin : $(N2).o
#	$(LD) $(LDFLAGS) -o $@ $<
#
#$(N2).o : $(N2).asm
#	$(ASM) $(ASMFLAGS) -o $@ $<
#
#$(N2).asm : $(N2).dguu
#	cp $< $@









#$(BIN) : $(OBJS)
#	$(LD) $(LDFLAGS) -o $(BIN) $(OBJS)
#logo.o : logo.asm
#	$(ASM) $(ASMFLAGS) -o $@ $<
#
#logo.asm :logo.dguu
#	$(CP) $(CPFLAGS) $< $@
#
#coop.o : coop.c
#	$(CC) $(CFLAGS) -o $@ $<
#
#coop.c : coop.dguu
#	$(CP) $(CPFLAGS) $< $@





.PHONY : boot
boot : boot.bin
	dd if=$< of=g.img conv=notrunc count=1

boot.bin : boot.asm
	$(ASM) -f BIN $(<) -o $@

boot.asm : boot.dguu
	$(CP) $(CPFLAGS) $< $@

#执行CP时 会检测参数文件的日期 ,以决定是否更新 , 这是Make的特性
#若是要执行的命令行开头不是TAB , 顺序轮到时 将不会执行 ...
#



.PHNOY: move
move:
	mount -o loop g.img /media/floppy
#	cp $(N1).bin /media/floppy/
	cp $(N2).bin /media/floppy/
	sleep 2
	umount /media/floppy




#0647N1716
#1032N1916
#negiup
