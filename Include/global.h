//


#ifndef global_DGUU
#define global_DGUU


//
//这样也许可以保证 , 当多个 对象文件包含 global.h 时
// 这些变量只会被声明一次 .
//声明在 定义GLOBAL_VARIABLES_HERE 的 地方 .
//
//这可能是因为 编译器逐个编译对象文件时 , 不会记忆上一个文件的预处理.
//
//

#ifdef GLOBAL_VARIABLES_HERE

#undef EXTERN
#define EXTERN

//这貌似是表示 预处理不能双重定义 覆盖 .
//但宏会按最后一次编译的来决定.
//虽然可能会有警告 .
//
// gcc 预处理 如此, 而nasm 直接错误 .
//
// 
// 对于宏而言 , 一个宏可以引用其后定义的一个宏
// 预处理会在编译代码前 进行 .
// 

#endif



EXTERN int ticks ;



EXTERN int disp_pos ;
EXTERN u8 gdt_ptr[6] ;
EXTERN DESCRIPTOR gdt[GDT_SIZE] ;
EXTERN u8 idt_ptr[6] ;
EXTERN GATE idt[IDT_SIZE] ;

EXTERN int scll ;  
//Merely for tty.dguu . Declared global variable for Linux C special
// 


EXTERN TSS tss ; 
  // 这么声明结构 不会初始化
EXTERN PROCESS*	p_proc_ready ;

EXTERN u32 k_reenter;

EXTERN int nr_current_console;


extern PROCESS proc_table[] ;
extern char task_stack[] ;

extern TASK            task_table[] ;
extern irq_handler	irq_table[] ;
extern system_call sys_call_table[];


extern TTY tty_table[];
extern CONSOLE console_table[];


#endif

//1003D616
//negiup
