
//

#ifndef set8253_DGUU
#define set8253_DGUU


//时钟中断 
//由一个被称作 PIT Programmable Interval Timer 的芯片来触发的 .
// 在IBM XT中 , 这个芯片用的是Intel 8253 , 在 AT 以及以后换成了 Intel 8254 .
//8254功能更强一些 , 但对于增强的功能 , 我们并不一定涉及 , 所以我们只是称呼为 8253
//
//8253有3 个计数器 counter , 它们都是16bit 的 ,各有不同的作用 ,如
//
//      COUNTER         EFFECT
//      Counter0        输出到IRQ0 , 以便每隔一段时间让系统产生一次时钟中断
//      Counter1        通常被设为18 , 以便大约每 15 us 做一次 RAM刷新
//      Counter2        连接PC喇叭
//
//
//因此 时钟中断实际上由8253的Counter0 产生 .
//
//计数器的工作原理 :
// 它有一个输入频率 , 在 PC 上是 1193180 Hz .
// 在每个时钟周期 (CLK cycle) , 计数器值会减1 , 当减到0时 , 就会触发一个输出 .
// 由于计数器是16bit的 , 所以最大值是 65535 ,  
// 因此 , 默认的时钟中断的发生频率是 1193180 / 655436 ~= 18.2 HZ
//
//我们可以通过编程来控制8253 , 因为如果改变计数器的计数值  , 那么中断产生的时间间隔也
//就相应改变了 . 
//  
//比如, 如果想让系数每10ms产生一次中断 , 也就是让输出频率为 100 hz ,
//那么需要为计数器赋值为1193180  / 100 ~= 11931 
//
//  输入频率为  1193180 Hz , 而周期为 1/1193180 s 
//  (  1/119380 * T )^(-1)  = 100
//  T为输出周期 , 1/T = FREQUENCY ,  量纲 S^(-1) = HZ
//
//可以通过写端口来改变8253计数器值
//
//   8253 PORT
//   40h        8253 Counter0
//   41h        8253 Counter1
//   42h        8253 Counter2
//   43h        8253 模式控制寄存器 (Mode Control Register)
//
//改变Counter0计数值要操作端口 40 h .
// 但是这个操作稍微有一点复杂 , 因为我们需要先通过端口 43h 写 8253 模式控制寄存器 .
// 先来看一下它的数据格式 ,
// 
//   Mode Control Register : PORT 43h
//   BIT
//   7,6         计数器选择位 同时也是8254的Read Back 命令位
//   5,4         读 写 锁位
//   3,2,1       计数器模式位
//   0           计数器进制设置   
//
//   计数器进制设置位
//   BIT0 = 0 计数器使用二进制 ,
//   BIT0 = 1 计数器使用BCD码 
//   
//   计数器模式位
//   BIT 3  2  1        MODE            NAME
//       0  0  0        0               interrupt on terminal count
//       0  0  1        1               programmable one-shot
//       0  1  0        2               rate generator
//       0  1  1        3               square wave rate generator
//       1  0  0        4               software triggered strobe
//       1  0  1        5               hardware triggered strobe
//  
//   读写锁位
//   BIT 5  4           DESCRIBE
//       0  0           锁住当前记数值 (以便于读取)
//       0  1           只读写高字节
//       1  0           只读写低字节
//       1  1           先读写低字节 , 再读写高字节
//   锁住 当前计数器值并不是让计数停止 , 而仅仅是为了便于读取 . 
//   相反 , 如果不锁住直接读取会影响计数 .
//
//   计数器选择位
//   BIT 7  6           DESCRIBE
//       0  0           选择 Counter0
//       0  1           选择 Counter1
//       1  0           选择 Counter2
//       1  1           对8253而言非法 , 8254 是 Read Back 命令
//   
//随后对 模式控制寄存器 通过端口的 赋值为
//  00 11 010 0
//即 设置Counter0 寄存器 , 
//   高低都写 , rate generator Mode , 以及Counter0 使用 二进制 .
//


#include "tosh.h"



/* 8253/8254 PIT (Programmable Interval Timer) */
#define TIMER0         0x40 /* I/O port for    timer channel 0 */
#define TIMER_MODE     0x43 /* I/O port for timer mode control */

#define RATE_GENERATOR 0x34 /* 00-11-010-0 :
	     * Counter0 - LSB then MSB - rate generator - binary
     */

#define TIMER_FREQ     1193182
   /* clock frequency for timer in PC and AT */

#define HZ             100  
  /* clock freq (software settable on IBM-PC) */




void set8253()
{

  out_byte(TIMER_MODE, RATE_GENERATOR);

  out_byte(TIMER0, (u8) (TIMER_FREQ/HZ) );
  out_byte(TIMER0, (u8) ((TIMER_FREQ/HZ) >> 8));
  //  TIMER0 , 0x40 , 端口 , 是设置 时钟输入输出调节计数器的值 . 

  put_irq_handler(CLOCK_IRQ, clock_handler) ; 
  enable_irq(CLOCK_IRQ);                     



  
}

#endif

//1032D2016
//negiup
