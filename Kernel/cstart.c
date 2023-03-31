//

//
// C中的函数中的 标签 , 一般只应用于 goto 语句, 而且除了goto语句可以引用外 似乎没有其他任何可以再引用的地方或理由 .
//  好比 给变量赋值 或是作为函数参数使用 goto的标签都是 错误的语法 . 
// 所以 这里的标签只能用于goto , 所以C中的标签一般没有太多使用的价值 . 难以达到和ASM那样预期的效果 ...
// 

//
//静态 static 函数 与 非static函数 具有同一个函数名时 被认为是两个函数 , 但函数名一致 所以会冲突 .
// 在一个函数 中调用了另一个函数 , 编译器会去上边找这个函数的函数生明 , 若是没有(有函数体但无声明时)那么会默认添加一个
// 以调用时的标签命名的函数名且具有调用时所提供的参数类似的形参类型 , 但这个函数为 非static .
//当一个函数声明后 , 编译器会允许连接器在外部寻找这个函数名对应的函数, 但编译器首先会在该文件寻找 .
//若是找到了有此函数名的函数体则对应的调用此函数体 , 但若是发现拥有此函数名的函数体有多个则报错,
//并且 声明为 static的 且具有此函数名的函数体 会被报告与函数声明冲突 , 因为函数声明默认 非static .
//
//如果编译器去上边寻找具有这个函数名的函数声明或函数体时 , 如果有个函数名的任何函数体或函数声明 , 则不会创建函数声明.
//
//
//
//且若是调用static函数那么 函数声明就变得必要, 因为编译器会以为此函数名为非static , 
//而 同名的 static 或 非static函数属于两个函数 且冲突.
//
//
//所以最好为每个调用的函数提供一个函数声明 以免编译器默认添加 . 
//
//那么 static函数的函数声明一定要有 static.
//
//函数体也许不会总是出现在调用函数之前 , 所以出现在后边以 函数声明 似乎是简单的 清楚的...
//
//




//
//后缀自增 
//  优先级低于赋值运算 属于最低优先级(如果分号 ; 不算运算符)
//

//
//
// C 编译 某个 .o文件时 , 如果所调用的函数 没有 函数声明 ,
// 那么 可以的话 C会默认的添加一个函数声明(也许可以根据调用参数辅助 或是概化处理) ,
// 即假设所调用的这个函数
// 会在其他.o文件中 , 直到 连接 ld 时 , 如果 ld找不到 则会以错误结束 .
//
// 函数声明提供一种检查   , 所以声明为佳 .
//
//

//
//全局变量(或且声明为static)初始化为 0 时 , 相当于没有初始化
//仍然在bss段 , 只有初始化为非0的其他值 才不会在bss段
//
// 在Linux C的static 变量 不会初始化 依然在 Bss段 
// 所以 Linux C 中 static全局变量 与 全局变量 都在 bss段.
// 只是 static 具有私有性质对链接器 .
//
//

//
//C的一个特性 就是会对 强制转型的对象 进行适当的扩展或压缩
//而不让这种语言特性 只是摆设 ...
// 

//
//对于使用 include 的文件名 , 不应该在多个搜索目录中同时出现
//此时 即便扩展名不同(.c或.h), 只要文件名前缀相同(如 global.) , 都会视为相同的文件
//gcc会按搜索目录的顺序包含其中一个 , 这就可能会发生错误 ...
//
//也许 include 会把 .c 与 .h 不加区分 ...
//
//保证include的文件名在各个搜索目录独有 ...
//

//gcc 对于 include的 指令会相对于使用gcc的目录 , 或是 给出 -I 增加相对目录
// 在或者如果 编译的文件 有一个相对目录  那么 include 也会相对这个目录 ,
// 如 gcc -c /Kernel/cstart.c 
//cstart.c中的include 会再去 /Kerenl目录 找找看 ...
//

//
// 不能嵌套 多段注释 .
// c会以遇到的 第一个多段注释的结尾符号 为一个多段注释
// 这导致了 错配 ， 但这是c syntax .
//

//
//c 中全局变量的声明 没有指定初始化时 , 是一个随机值 .
//像是 bss 段 的 成员  . 
//


//
// C 声明的函数 默认都是 global
// 对于非global 即 其他对象文件不可使用的标签. 要声明为 static
// C 的函数声明默认 相当于 extern function_label
// 但这种的extern会表示 可能在其他对象文件中 , 或是 就在本对象文件中. 
// 由链接器以此来找.  
// 变量声明为 extern 的 效果类似 函数声明
// 但变量默认为global .
// 
// 
// 
// asm 的 任何标签 默认相当于 C 的 static
// 需要明确 global , 因为没有函数声明 , 所以外部的标签需要 extern  .
//
// 这里的C 为 GCC , asm 为 nasm
//
// void * 指针 可以接收 任何类型的指针,  且可以接收 NULL即 0 ,
//  这些情况 编译器不会有警告 .
//
//
// TASK task_table[];
// int t ;
//
//  p = & task_table[0] ;  
//  p = task_table;
//  p = & task_table ;
//  p = & task_table[0] ;
//
//  4 个 p 等值 .
//  指针接收一般类型 只是会警告 , 但接收结构类会错误
//  结构类型是一个特殊的类型 , 
//  结构只能引用到一般类型的成员 才会得到 有定义的值 ,
//  这涉及如何访问内存. 
//  
//  变量表示一块内存 , 当它的大小size确定时 (也就是类型确定时) , 
//  对变量取(或赋)值将是这块内存按这种访问方式得到或给予的值
//  而每个变量都有一个地址 .
//  变量如何访问以及 地址如何 被所在平台取决 . 
//  
//
//  结构赋值 应该只能赋值给同类型结构 .
//
//  地址 加1 一般都是以这个地址的类型作为size 来增加
//  若是结构地址 + 1 那么 会被当作结构数组处理 .
//  所以结构成员的引用通过结构地址 要么使用指针操作符 ->  , 
//  要么通过取消引用操作符 * 得到结构的值 再以引用操作符 . 引用其成员 .
//  也应该这么做 , 强制转型结构地址再操作似乎不太好.
//
//
//  数组是常量指针, 只是操作特殊 , 数组的每个对象都是一个变量 , 与结构不同的是
//  数组如何访问是确定的 .
//  正如数组所对应的地址是一组数据 若是数组指针改变 , 这组数据可能丢失 .
//  数组的操作方式也许指针也可以那么做.
//  常量指针 即 指向的内存区不会变 , 但内存区的内容可以改变.
//  还有 指针常量 其效果与常量指针相反 ,还是 常量指针常量 .
// 
//  数组的下标操作方式 得到的 都是 *(地址+下标) 也就是变量 ,
//  变量只有在 取值或赋值 时 才能体现出 变量的意义 , 所以 说 *(地址+下标)是变量 也需要在赋值或取值时体现. 
//  
//  变量取或赋值时 可以取地址或对变量的内容当作指针并且指针的访问方式访问以求得到那个地址的内容 , 
//  这要求这个变量是一个指针 . 
//

//
// C 对 没有给出函数声明 而直接调用的函数 , 会给出对应的函数声明 ,
// 但如果在链接时没有对应的函数体 那么 错误 .
// 所以 C 中调用不存的函数 而编译错误时 是在链接时 .
//
//

//
//在Linux上 C 声明的 全局变量会在 bss段 , 而bss段的数据不会出现在文件中 ,
// 由ELF在内存中出现 . 
//ELF为bss段保留了一些对应程序头或节头 , 加载后bss会出现在内存中.
//
//bss段的数据不会自动初始化 正如它不需要占用文件空间 ,
// 但 C 允许对全局变量的初始化 , 那么也许 C 会添加一些代码来初始化 . 
//
//

//

#define GLOBAL_VARIABLES_HERE



#include "tosh.h"


#include "type.h"
  //预处理指令 需要小写 ...

#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"





PUBLIC PROCESS proc_table[NR_TASKS];
PUBLIC char task_stack[STACK_SIZE_TOTAL] ;

PUBLIC TASK task_table[NR_TASKS] = {
  {task_tty , STACK_SIZE_TESTA , "tty" } ,
  {testB , STACK_SIZE_TESTB , "testB" } ,
  {testC , STACK_SIZE_TESTC , "testC" } ,       
  {testA , STACK_SIZE_TTY , "testA" } } ;

//struct array initial 
//

//
//   #define NR_TASKS	4  , it's in ../const.h 
//

PUBLIC irq_handler irq_table[NR_IRQ];      
  //typedef	void (*irq_handler) (int irq);  , it is in "../include/type.dguu " 

PUBLIC system_call sys_call_table[NR_SYS_CALL] = 
       { sys_get_ticks , sys_write};
   // typedef void* system_call ;  , it is in " ../include/type.h " 
   //   #define NR_SYS_CALL 1

PUBLIC TTY tty_table[NR_CONSOLES] ;        //NR_CONSOLES = 3 
PUBLIC CONSOLE console_table[NR_CONSOLES] ;
// #define NR_CONSOLES	3	/* consoles */   , it's in ../include/const.h


static int test = 10 ;

PUBLIC void cstart() 
{
  u16* p_gdt_limit = ( u16* ) ( &gdt_ptr[0]) ;
  u32* p_gdt_base = ( u32* ) ( &gdt_ptr[2] ) ;
  u16* p_idt_limit = (u16*) (& idt_ptr[0] ) ;
  u32* p_idt_base = (u32*) (& idt_ptr[2]) ;

  _feed(&disp_pos);

  _appear4( &disp_pos ,
            "-----------------cstart domian-------------" , 
            (void * )test) ;  //third argument for test

  _str_cpy( &gdt , 
            (void* ) ( * ( u32* ) ( & gdt_ptr[2] ) ) , 
            * ( (u16*) ( &gdt_ptr[0] )) + 1 ) ;

//
//  u16* p_gdt_limit = ( u16* ) ( &gdt_ptr[0]) ;
//  u32* p_gdt_base = ( u32* ) ( &gdt_ptr[2] ) ;
// that c init local variable should be before any code 
// but gcc don't give warning.
//
  
  *p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
  *p_gdt_base = (u32) &gdt ; 
  *p_idt_limit = IDT_SIZE * sizeof(GATE) -1 ;       
  *p_idt_base = (u32)&idt ;

  init_prot();



  
} 


//1420D316
//1121D816
//1808D1116
//2030D2516
//1904FEB1818
//negiup

