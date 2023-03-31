;


;
;BIT16 与BIT32 对代码的解析 一致于 处理器 在RM 和 PM下 对代码的解析 
; 好比 Intel 在PM下执行BIT16的32bit代码 导致错误 . 
;  但在RM下 执行BIT16的32bit代码 无误 ... 
;
;


jmp rm_start


;*********************************************

[SECTION .stact]
                                stack:
TIMES 0x600 DB 0
RMSTACKTOP EQU $
PMSTACKTOP EQU BASEOFLOADERPHYSICADDRESS + RMSTACKTOP







;**************************************************

[SECTION .data]
                                data :

;------------------------------------------------------------------

%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/Prelude/Boot/LC.asm"
%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/Prelude/Boot/pm.asm"


;------------------------------------------------------------


_szFilename DB "KERNEL  BIN" , 0

_szFormat DB "value %d " , 0

_szChkBuff :
TIMES 0x100 DB 0

_dwMCRNumber DD 0

_dwRAMSize DD 0

_dwPos DD 0

szChkBuff EQU BASEOFLOADERPHYSICADDRESS + _szChkBuff
dwMCRNumber EQU BASEOFLOADERPHYSICADDRESS + _dwMCRNumber
dwRAMSize EQU BASEOFLOADERPHYSICADDRESS + _dwRAMSize
szFormat EQU BASEOFLOADERPHYSICADDRESS + _szFormat
dwPos EQU BASEOFLOADERPHYSICADDRESS + _dwPos


;------------------------------------------
;
;4 GB 的 数据段 和 代码段 的 段描述符 似乎就是这些 
;段界限为 0xFFFFF , 因段界限仅占用 20 bit
;所以这种设计 DA_LIMIT_4K 成了需要 ,
;而4K对齐, 4K是页的一般大小 ...
;DA_32 也为 32bit .
;
;

gdt :
  gdtEmpty Descriptor 0 , 0 , 0
  gdtFlatC Descriptor 0 , 0xFFFFF , DA_CR | DA_32 | DA_LIMIT_4K
  gdtFlatRW Descriptor 0 , 0xFFFFF , DA_DRW | DA_32 | DA_LIMIT_4K
  gdtVedio Descriptor 0xB8000 , 0xFFFF , DA_DRW | DA_DPL3

  ;DA_32 对于 gdtFlatRW 主要为 加载到ss时的属性 , 
  ; 对于其他 数据段 因其 DA_DRW 为 Extend-up 属性故 DA_32 位 对这里的其他数据段没有作用 . 
  ;

gdt_LENGTH EQU $ - gdt 

gdtPtr DW gdt_LENGTH - 1
       DD BASEOFLOADERPHYSICADDRESS + gdt


SELECTORFLATC EQU gdtFlatC - gdt
SELECTORFLATRW EQU gdtFlatRW - gdt
SELECTORVEDIO EQU (gdtVedio - gdt) | SA_RPL3












;******************************************

[SECTION .code16]
[BITS 16]

                                        rm_start :
mov ax , cs
mov ds , ax
mov es , ax
mov ss , ax
mov fs , ax
mov gs , ax     
mov sp , RMSTACKTOP

push _szChkBuff
call _mem_size_info
add sp , 2
mov [_dwMCRNumber] , ax

call _floppy_driver_reset

push _szFilename
push BASEOFKERNELFILE
push OFFSETOFKERNELFILE
call _loadFile
add sp , 6

call _kill_motor

lgdt [gdtPtr]
cli
in al , 0x92
or al , 0x2
out 0x92 , al

mov eax , cr0
or eax , 1
mov cr0 , eax   

jmp DWORD SELECTORFLATC : BASEOFLOADERPHYSICADDRESS + pm_start


%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_mem_size_info.asm"
%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_loadFile.asm"
%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_floppy_driver_reset.asm"
%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_kill_motor.asm"









;**************************************************

[SECTION .code32]
[BITS 32]

                                        pm_start :

mov ax , SELECTORFLATRW
mov ds , ax
mov es , ax
mov ss , ax
mov fs , ax
mov ax , SELECTORVEDIO
mov gs , ax

mov esp , PMSTACKTOP

mov DWORD [dwPos] , (80 * 1 + 0) * 2

call mem_size

push eax 
push eax
push szFormat
push dwPos
call _appear4
add esp , 12 


call paging_setup
add esp , 4

call init_kernel


jmp SELECTORFLATC : KERNELENTRYPOINTPHYSICADDRESS




;---------------------------------------------
init_kernel :
;
;
;
;
push ebp
mov ebp , esp

push esi

mov cx , [BASEOFKERNELPHYSICADDRESS + 0x2C]     ; e_phnum
movzx ecx , cx
mov esi , [BASEOFKERNELPHYSICADDRESS + 0x1C]    ; e_phoff
add esi , BASEOFKERNELPHYSICADDRESS

                                .2 :

mov eax , [esi + 0]                             
cmp eax , 0                                     ; null
jz .1

push DWORD [ esi + 0x10]                        ;p_filesz
mov eax , [esi + 0x4]                           ;p_offset
add eax , BASEOFKERNELPHYSICADDRESS
push eax
push DWORD [ esi + 0x8]                         ;p_vaddr
call _str_cpy
add esp , 12

                                .1 :
add esi , 0x20
loop .2

pop esi

mov esp , ebp
pop ebp
ret


 













;---------------------------------------------------
paging_setup :
;
;ebp + 8 : ram size
;
;
push ebp
mov ebp , esp

pushad

xor edx, edx
mov eax, [ebp + 0x8]
mov ebx, 400000h	
div ebx
mov ecx, eax	
test edx, edx
jz .no_remainder
inc ecx		
                                  .no_remainder:
push ecx		

	; 为简化处理, 所有线性地址对应相等的物理地址. 并且不考虑内存空洞.

mov edi, PAGEDIRBASE
xor eax, eax
mov eax, PAGETBLBASE | PG_P  | PG_USU | PG_RWW

                       .1:

stosd
add eax, 4096
loop .1


pop eax			
mov ebx, 1024	
mul ebx
mov ecx, eax	
mov edi, PAGETBLBASE
xor eax, eax
mov eax, PG_P  | PG_USU | PG_RWW

                              .2:
stosd
add eax, 4096	
loop .2

mov eax, PAGEDIRBASE
mov cr3, eax
mov eax, cr0
or eax, 80000000h
mov cr0, eax
jmp .3
                                .3 :
nop

popad

mov esp , ebp
pop ebp
ret





;----------------------------------------------------
mem_size :
;
;
;

push ebp
mov ebp , esp

sub esp , 4

push esi

mov DWORD [ebp - 0x4] , 0

mov esi , szChkBuff
mov ecx , [dwMCRNumber]
                                .2 :
mov eax , [esi + 0x10]
cmp eax , 1
jnz .1
mov eax , [esi]
add eax , [esi + 8]
cmp [ebp - 0x4] , eax
jae .1
mov [ebp - 0x4] , eax

                                .1 :
add esi , 0x14
loop .2

mov eax , [ebp - 0x4]

pop esi


mov esp , ebp
pop ebp
ret




%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_appear4.asm"
%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_str_cpy.asm"






;2206D416
;negiupp
