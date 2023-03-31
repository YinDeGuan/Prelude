//

#ifndef prot_DGUU
#define prot_DGUU

//***********************************************
// library part
                                                //glib.asm
PUBLIC void out_byte(u32 port, u32 value) ;

PUBLIC u32 in_byte(u32 port) ;

PUBLIC void _appear4(int* disp_pos , char* szFormat , void* varg) ;

PUBLIC void _feed( int * disp_pos) ;

PUBLIC void _str_cpy(void* des , void* src , int len) ;

PUBLIC void _str_set(void* des , int byteNumber , int size) ;

PUBLIC int disable_irq(int irq) ;

PUBLIC void enable_irq(int irq) ;

PUBLIC void disable_int() ; 

PUBLIC void enable_int() ;

PUBLIC void _i_to_h(char* buffer , int num) ; 
PUBLIC void _i_to_d(char* buffer , int num) ; 




                                                //glib2.asm
PUBLIC void delay(int time) ; 





//*******************************************
// un library part

                                //../Kernel/protect.c
PUBLIC void init_prot() ;
PUBLIC u32 seg2phys(u16 seg) ;


                                //../Kernel/i8259.c
PUBLIC void init_8259A() ;
PUBLIC void put_irq_handler(int irq, irq_handler handler);
PUBLIC void spurious_irq(int irq);


                                //../Kernel/kernel.asm
PUBLIC void restart() ;
PUBLIC void save() ;           //no call by other file 
PUBLIC void enter_r3();
PUBLIC void leave_r3();



                                //../Kernel/main.c
PUBLIC void testA() ;
PUBLIC void testB();
PUBLIC void testC();

                               // ../Kernel/clock.c
PUBLIC void clock_handler(int irq);
PUBLIC void milli_delay(int milli_sec) ;


PUBLIC int sys_get_ticks() ; // ../Kenrel/proc.c
PUBLIC void schedule() ; 

                             // ../Kernel/protect.c
PUBLIC void init_descriptor(DESCRIPTOR * p_desc , u32 base , u32 limit , u16 attribute ) ; 
PUBLIC void init_call_gate(GATE * p_gate , u32 offset , u16 selector , u16 attribute ) ; 

PUBLIC  void    sys_call();         // ../Kernel/kernel.c

PUBLIC int get_ticks();        
PUBLIC void write(char* buf , int len ) ; 
                                // ../Kernel/syscall.asm


PUBLIC void init_keyboard() ;       // ../Kernel/set8052.c
PUBLIC void keyboard_read(TTY* p_tty); 


PUBLIC void task_tty() ;          
PUBLIC void in_process(TTY* p_tty , u32 key);
PUBLIC int sys_write(char* buf , int len , PROCESS* p_proc) ; 

                   
                                          // ../Kernel/tty.c



PUBLIC void update_cursor(int position) ;       //  ../Kernel/meet_vga.c   
PUBLIC void scroll(int line) ;   
PUBLIC void set_vedio_start_addr(u32 addr);



                                     // ../Kernel/set8253.c
PUBLIC void set8253();

                                     //  ../Kernel/console.c
PUBLIC int is_current_console(CONSOLE* p_con);
PUBLIC void out_char(CONSOLE* p_con , char ch);
PUBLIC void select_console(int nr_console) ;
PUBLIC void scroll_screen(CONSOLE* p_con , int direction);
PUBLIC void init_screen(TTY * p_tty) ; 



PUBLIC int printf( const char * fmt , ... ) ; 
                                            // ../Kernel/printf.c
                                            // const char* fmt  常量指针 , 指针不可改变指向 . 
                                            // 留意 这样可变参数声明 . 


PUBLIC int vsprintf(char* buf , const char* fmt , va_list args) ;

       //..Kernel/vsprinf_g.c




#endif

//1014D616
//negiup
