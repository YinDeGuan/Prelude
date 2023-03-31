;



%include "sconst.inc"


INT_VECTOR_SYS_CALL equ 0x90
_NR_get_ticks       equ 0
_NR_write	    equ 1

global get_ticks
global write

extern p_proc_ready

[SECTION .text]
[bits 32]

;----------------------------------------------------------
get_ticks :
;
; int interface for C 
; 
; 这段代码将被r1 执行 , 而中断操作可以执行 因之前在r0 已将  IOPL = 1 
;
mov eax , _NR_get_ticks
int INT_VECTOR_SYS_CALL
ret




;----------------------------------------------------
write:
;
;  void write(char *    buf , int len) ; 
;

push ebp
mov ebp , esp 

push ebx
push ecx

mov ecx, [ebp + 12]
mov ebx, [ebp + 8]
mov eax, _NR_write
int INT_VECTOR_SYS_CALL

pop ecx
pop ebx

mov esp , ebp
pop ebp
ret         



;1602D1816
;negiup
