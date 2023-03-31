//

#include "tosh.h"

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"

PUBLIC int kernel_main()
{
	

//PUBLIC char task_stack[STACK_SIZE_TOTAL] ;
//
//PUBLIC TASK task_table[NR_TASKS] = {
//  {task_tty , STACK_SIZE_TESTA , "tty" } ,
//  {testB , STACK_SIZE_TESTB , "testB" } ,
//  {testC , STACK_SIZE_TESTC , "testC" } ,       
//  {testA , STACK_SIZE_TTY , "testA" } } ;

PROCESS* p_proc	= proc_table;
char* p_task_stack = task_stack + STACK_SIZE_TOTAL ;
TASK* p_task = task_table ;
u16 selector_ldt = SELECTOR_LDT_FIRST;
int i = 0;

_feed(&disp_pos) ;

_appear4( &disp_pos , " ldt_selector %d ..." , & (p_proc->ldt_sel)) ;
_appear4( &disp_pos , " ...%d ..." , & (p_proc->ldts[0])) ;



for(i = 0 ; i < NR_TASKS ; i++)
{
  _str_cpy(p_proc->p_name , p_task->name , _str_len(p_task->name) +1 ) ;
  p_proc->pid = i ;

  if( !i ) 
  {
    p_proc->nr_tty = 0 ; 
  }
  else 
  {
    p_proc->nr_tty = 1 ;
  }

  p_proc->ldt_sel = selector_ldt;

  _str_cpy( &p_proc->ldts[0] , &gdt[SELECTOR_KERNEL_CS >> 3] , sizeof(DESCRIPTOR) ) ;
  p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;

  _str_cpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR) ) ;
  p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;

  p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
  p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
  p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
  p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
  p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
  p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK)   | RPL_TASK;

//  /* 选择子类型值说明 */
//  /* 其中, SA_ : Selector Attribute */
// #define	SA_RPL_MASK	0xFFFC
// #define	SA_RPL0		0
// #define	SA_RPL1		1
// #define	SA_RPL2		2
// #define	SA_RPL3		3
// #define	SA_TI_MASK	0xFFFB
// #define	SA_TIG		0
// #define	SA_TIL		4
//
//#define	RPL_KRNL	SA_RPL0
//#define	RPL_TASK	SA_RPL1
//#define	RPL_USER	SA_RPL3
// these are in ./const.h
// 




  p_proc->regs.eip = (u32)p_task->initial_eip;
  p_proc->regs.esp = (u32)p_task_stack;
  p_proc->regs.eflags = 0x1202; // IF=1, IOPL=1 , 第2bit总是 置位 .
                                // 中断开 

  p_task_stack -= p_task->stacksize;
  p_proc++;
  p_task++;
  selector_ldt += 1 << 3;
  
}


  k_reenter = 0 ;             //init to 0 is important ; cooperate kernel.c 
  ticks = 0 ;

  scll = 0 ;  //cooperated tty.dguu 

  proc_table[0].ticks = proc_table[0].priority =  7;   //tty cpu times
  proc_table[1].ticks = proc_table[1].priority =  5;
  proc_table[2].ticks = proc_table[2].priority =  3;

  proc_table[3].ticks = proc_table[3].priority =  5;       

  p_proc_ready = proc_table ;
   //   EXTERN PROCESS*	p_proc_ready ;
   //  it's in ../global.h 

  _feed(&disp_pos) ;


  set8253();

//0x00031b94 
//0x00031700

  restart() ;

  while(1){}        // interrupt bit is 0 until now 



}




//
//随后三个过程分时执行后 disp_pos 是不安全的 .
//我试图使用 局部变量 但 _appear4 本就是不安全的 .
//so just is ...
//


void testA()
{
  int i = 0;

  while(1)
  {
    //i ++ ;
    //_appear4( &disp_pos , "A:%d" , (void*) get_ticks()) ;
    
    milli_delay(1000) ;
    printf("say cdg %x" , sizeof(unsigned short) );
  }

}


void testB()
{
  while(1) 
  {
    //_appear4(&disp_pos , " B:%d" , (void*) get_ticks() );
    //milli_delay(100) ;
    
  }
}


void testC()
{
  int i = 0;
  while(1) 
  {
    //i ++ ;
    //_appear4(&disp_pos , " C:%d" , (void*) i);
    //milli_delay(100) ;
  }


}

//1258D1516
//negiup
