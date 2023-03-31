;

; cs ds es fs gs 这些段寄存器 只能赋值 非系统段描述符选择子  也就是
;  赋值的选择子对应的段的 S标志为 1 .
; 且 只要非系统段描述符是可读的 就可以赋值到 ds es fs gs 这些段寄存器
; ss有些特殊 其对应的段描述符一定是可写的 .
; 
; 赋值到段寄存器时 一般要先赋值到 通用寄存器再赋值到段寄存器 ... 这就是好的惯例 .
;  也算是因Intel特性而定的 ...
; 
; 使用调用门时  好比  call DWORD SELECTOR_GATE_R3TOR0 : GOAL ,
;   此命令将忽略 GOAL 也就是 GOAL = 0 或 -0 是没有区别的.
; 也就是要依赖于 门描述符中的OFFSET , 门描述符中的OFFSET才是我们需要的GOAL
; 顺便说下该指令的地址布局 , 
;  首先是 call指令码 然后是低4位GOAL 再然后是高2位 SELECTOR_GATE_R3TOR0
; 一般指令的最前面的字节都是指令码 ... 这也似乎符合机器设计 .
;
; 

 
; 


;
;nasm 提供了 这样 >> , | 类似C的运算符 , 可在编译期由nasm运算
; 


; ld 的 -Ttext 选项重置 start标签所处偏移量 , 且源文件中的其他标签相对start标签偏移量
; 对于elf文件, 无论其如何安排 , 最后以loader.dguu中的加载算法 , 
; 总可以将start加载到 -Ttext所设置的地址
; 
;  

;
;端口操作 out , in
;可以使用 8位立即值作为port num, 或是 包含立即值的dx 作为port num ,
;但dx成为16bit  . 端口操作 总是需要寄存器 al 作为输入 或者 输出 
;常量或是dx 是in或out仅仅允许的端口号 表达.
;


;
; 通过中断门 从低特权到高特权变换时
; 中断门DPL被忽视 , 低特权CPL= RPL  .
; 高特权目标DPL <= CPL .
; 如此从高特权中断iretd返回时 CPL=RPL .
; 
; LDT描述符的 DPL 也被忽略 .
; 若是把LDT视作门描述符 ,但 它的结构段描述符 .
; 所以LDT是特殊的.
;

;
;对于使用寄存器寻址 , 支持 寄存器 与 一个常量相乘的积 再与(可选的)一个常量相加的 寄存器索引寻址操作 ,
;这应该支持体现在编码上 .
;
;

;
;一般保护错误 包括 对所开启的页总数的 越界 .
;

;SELECTOR_KERNEL_CS EQU 8
;NASM 不允许宏多重定义 , 
;也就是宏不会被覆盖 . 
;

%include "sconst.inc"
; 一般编译器的默认目录 会相对于 使用编译器时 所在目录
; 所以这里使用编译器给出的 -I path 选项 提供更多的搜索路径 .
;

extern gdt
extern cstart
extern	exception_handler
extern	spurious_irq
extern kernel_main
extern toyou


extern	gdt_ptr
extern	idt_ptr
extern	disp_pos
extern p_proc_ready
extern tss
extern irq_table
extern task_table
extern task_stack
extern proc_table
extern sys_call_table


extern k_reenter


[SECTION .bss]
StackSpace RESB 0x600
STACKTOP :
StackSpace_R3 RESB 0x800
STACKTOP_R3 : 


[SECTION .data]
strTest DB " try your code " , 0 



[SECTION .text]

global _start
global sys_call
global restart
global enter_r3
global leave_r3


global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error
global  hwint00
global  hwint01
global  hwint02
global  hwint03
global  hwint04
global  hwint05
global  hwint06
global  hwint07
global  hwint08
global  hwint09
global  hwint10
global  hwint11
global  hwint12
global  hwint13
global  hwint14
global  hwint15



                                _start :

mov esp , STACKTOP

xor eax , eax 
mov [disp_pos] , eax

sgdt [gdt_ptr]

call cstart

lgdt [gdt_ptr]
lidt [idt_ptr]

jmp SELECTOR_KERNEL_CS : csinit

                                csinit :

;push 0
;popfd

;sti    ;cli in ../Boot/loader.dguu
;ud2

mov ax , SELECTOR_TSS      ;TSS 的使用 也需要从 GDT引用 
ltr ax

;call toyou
;jmp $

jmp kernel_main  ;no back


















;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; hwardawre interrupt part
;




;------------------------------------------------

%macro  hwint_master    1

call save

in al , INT_M_CTLMASK
or al , (1 << %1)		;  | 屏蔽当前中断
out INT_M_CTLMASK , al	;

mov al , EOI			; 置EOI位
out INT_M_CTL , al		;

sti	                        ; CPU在响应中断的过程中会自动关中断，这句之后就允许响应新的中断

push %1
call [irq_table + 4 * %1]	;  | 中断处理程序
pop ecx			

cli

                                ;关中断为随后代码能快速进行 这可能与 端口的读与写 应该快速进行
                                ;也许当读或写端口时 会导致短时间内下次端口操作可以在更短时间内完成.

in al , INT_M_CTLMASK	
and al , ~(1 << %1)		;  | 恢复接受当前中断
out INT_M_CTLMASK , al	

ret


%endmacro










;---------------------------------------------------
ALIGN   16
hwint00:                ; Interrupt routine for irq 0 (the clock).
hwint_master 0                        

ALIGN   16
hwint01:                ; Interrupt routine for irq 1 (keyboard)
        hwint_master    1

ALIGN   16
hwint02:                ; Interrupt routine for irq 2 (cascade!)
        hwint_master    2

ALIGN   16
hwint03:                ; Interrupt routine for irq 3 (second serial)
        hwint_master    3

ALIGN   16
hwint04:                ; Interrupt routine for irq 4 (first serial)
        hwint_master    4

ALIGN   16
hwint05:                ; Interrupt routine for irq 5 (XT winchester)
        hwint_master    5

ALIGN   16
hwint06:                ; Interrupt routine for irq 6 (floppy)
        hwint_master    6

ALIGN   16
hwint07:                ; Interrupt routine for irq 7 (printer)
        hwint_master    7






%macro  hwint_slave     1
push    %1
call    spurious_irq
add     esp, 4
hlt
%endmacro




ALIGN   16
hwint08:                ; Interrupt routine for irq 8 (realtime clock).
        hwint_slave     8

ALIGN   16
hwint09:                ; Interrupt routine for irq 9 (irq 2 redirected)
        hwint_slave     9

ALIGN   16
hwint10:                ; Interrupt routine for irq 10
        hwint_slave     10

ALIGN   16
hwint11:                ; Interrupt routine for irq 11
        hwint_slave     11

ALIGN   16
hwint12:                ; Interrupt routine for irq 12
        hwint_slave     12

ALIGN   16
hwint13:                ; Interrupt routine for irq 13 (FPU exception)
        hwint_slave     13

ALIGN   16
hwint14:                ; Interrupt routine for irq 14 (AT winchester)
        hwint_slave     14

ALIGN   16
hwint15:                ; Interrupt routine for irq 15
        hwint_slave     15










;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; intterupt and exception 
; or
; system interrupt
;

divide_error:
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	jmp	exception
single_step_exception:
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	jmp	exception
nmi:
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	jmp	exception
breakpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	jmp	exception
overflow:
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	jmp	exception
bounds_check:
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	jmp	exception
inval_opcode:
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	jmp	exception
copr_not_available:
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	jmp	exception
double_fault:
	push	8		; vector_no	= 8
	jmp	exception
copr_seg_overrun:
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	jmp	exception


;has error code

inval_tss:
	push	10		; vector_no	= A
	jmp	exception
segment_not_present:
	push	11		; vector_no	= B
	jmp	exception
stack_exception:
	push	12		; vector_no	= C
	jmp	exception
general_protection:
	push	13		; vector_no	= D
	jmp	exception
page_fault:
	push	14		; vector_no	= E
	jmp	exception




copr_error:
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	jmp	exception

exception:
	call	exception_handler
	add	esp , 4*2	
	hlt




;PROCESS 对象在这里起了很重要的作用 . 
;从restart的第一次执行 , 其第一句就已经表示 
; 对stack的使用要小心 , 否则不妨就是对PROCESS对象的破坏 . 
;


;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
restart:


mov esp, [p_proc_ready]    ;
lldt [esp + P_LDT_SEL] 
lea eax, [esp + P_STACKTOP]

mov DWORD [tss + TSS3_S_SP0], eax
               ;ss0 init in protect.dguu::init_prot() method


                            restart_reenter :

dec DWORD [k_reenter]

pop gs
pop fs
pop es
pop ds
popad

add esp , 4

iretd             ; interrupt set  





;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


save :


pushad  
push ds 
push es 
push fs  
push gs 


mov dx , ss
mov ds , dx
mov es , dx


mov esi , esp                    ;esi = 进程表起始地址

inc dword [k_reenter]            
cmp dword [k_reenter] , 0        
jne .1                           
mov esp , STACKTOP
push restart                     
jmp [esi + RETADR - P_STACKBASE] 


                                 .1:      ; 已经在内核栈，不需要再切换
push    restart_reenter                     
jmp     [esi + RETADR - P_STACKBASE]        
                                            






;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



sys_call :


call save      ;整个用户中断 会保存中断之前的寄存器环境
               ;但对于中断内部 一些寄存器并不安全 , 如果用其
               ;传递参数 应该小心留意 . 
               ;这里save 对于 eax ebx ecx edi是安全的.
               ;他们也应用于中断号 以及 中断所需要的参数 . 
               ;
sti

push dword [p_proc_ready]
push ecx
push ebx
call [sys_call_table + eax * 4 ]
add esp , 12 
mov [esi + EAXREG - P_STACKBASE] , eax
cli
ret 





%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_appear4.asm"



;0952N2616
;1307D1516
;1432D1716
;1904FEB1818
;negiup
