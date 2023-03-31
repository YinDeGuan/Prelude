//


//调整8253后 , 用变量 ticks 来 度量时间(间隔 )
//并给出了调度算法 , 虽然这种调度导致进程可以连续得到时间片 , 而不是离散以高机率得到时间片
//
//为了让  ticks = 1 对应 10 ms , 必要的让 时钟中断处理时间 < 10 ms , 
//宽松些 也应该 在10ms左右
//
//

#include "tosh.h"

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"

#define HZ 100



PUBLIC void clock_handler(int irq)
{
 
  ticks ++ ;
  p_proc_ready->ticks-- ;

  if(k_reenter != 0) 
    return;
 
  if( p_proc_ready -> ticks > 0 )
    return ;
  
  schedule() ;

}


//
// 一般 供 r1 调用
//

PUBLIC void milli_delay(int milli_sec)
{
  int t = get_ticks() ;
  while( ( (get_ticks() - t) * 1000 / HZ) < milli_sec) ;
}



//1728D1616
//negiup
