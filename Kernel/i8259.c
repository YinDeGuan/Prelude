//


#include "tosh.h"

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"




PUBLIC void init_8259A()
{

int i = 0;

//#define INT_M_CTL 0x20
//#define INT_M_CTLMASK 0x21
//#define INT_S_CTL 0xA0
//#define INT_S_CTLMASK 0xA1

/* Master 8259, ICW1. */
out_byte(INT_M_CTL , 0x11);

/* Slave  8259, ICW1. */
out_byte(INT_S_CTL,	0x11);

//Master 8259, ICW2. 设置 '主8259' 的中断入口地址为 0x20
out_byte(INT_M_CTLMASK,	INT_VECTOR_IRQ0);

//Slave  8259, ICW2. 设置 '从8259' 的中断入口地址为 0x28
out_byte(INT_S_CTLMASK,	INT_VECTOR_IRQ8);

//Master 8259, ICW3. IR2 对应 '从8259'.
out_byte(INT_M_CTLMASK,	0x4);

//Slave  8259, ICW3. 对应 '主8259' 的 IR2. 
out_byte(INT_S_CTLMASK,	0x2);

//Master 8259, ICW4. 
out_byte(INT_M_CTLMASK,	0x1);

//Slave  8259, ICW4.
out_byte(INT_S_CTLMASK,	0x1);
  


//Master 8259, OCW1.
out_byte(INT_M_CTLMASK,	0xFF);

// Slave  8259, OCW1. 
out_byte(INT_S_CTLMASK,	0xFF);
  // 这2者应该是屏蔽 8259A
  // 指令执行的速度 一般要比中断快的多 .        
  // 所以即使之前没有屏蔽中断 也是基本安全的 . 
  // 


for( i = 0 ; i < NR_IRQ ; i++) irq_table[i] = spurious_irq;

  // #define	NR_IRQ		16	/* Number of IRQs */
  
}

PUBLIC void spurious_irq(int irq)
{

  _appear4( &disp_pos , "spurious irq : %d " , (void*)irq) ;    
  
     //void* pointer agree to any type pointer

  _feed( & disp_pos) ;

  _appear4( &disp_pos , "spurious irq : %d " , (void*)irq) ;    

}

PUBLIC void put_irq_handler(int irq , irq_handler handler )
{
  disable_irq(irq) ;
  irq_table[irq] = handler ;
}


//1744D716
//negiup
