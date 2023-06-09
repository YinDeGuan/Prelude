//

#ifndef proc_DGUU
#define proc_DGUU

typedef struct s_stackframe {
	u32	gs;		/* \                                    */
	u32	fs;		/* |                                    */
	u32	es;		/* |                                    */
	u32	ds;		/* |                                    */
	u32	edi;		/* |                                    */
	u32	esi;		/* | pushed by save()                   */
	u32	ebp;		/* |                                    */
	u32	kernel_esp;	/* <- 'popad' will ignore it            */
	u32	ebx;		/* |                                    */
	u32	edx;		/* |                                    */
	u32	ecx;		/* |                                    */
	u32	eax;		/* /                                    */
	u32	retaddr;	/* return addr for kernel.asm::save()   */
	u32	eip;		/* \                                    */
	u32	cs;		/* |                                    */
	u32	eflags;		/* | pushed by CPU during interrupt     */
	u32	esp;		/* |                                    */
	u32	ss;		/* /                                    */
} STACK_FRAME ;


typedef struct s_proc {
STACK_FRAME regs;          /* process registers saved in stack frame */
u16 ldt_sel;               /* gdt selector giving ldt base and limit */




DESCRIPTOR ldts[LDT_SIZE]; /* local descriptors for code and data */
u32 pid;                   /* process id passed in from MM */
char p_name[16];           /* name of the process */

                                // LDT_SIZE = 2 
                                // sizeof(unsigned short) = 2 
int ticks ;
int priority ;     

int nr_tty ;                               





} PROCESS ;


typedef struct s_task 
{
  task_f initial_eip;       // typedef	void (*task_f) ();   , it is in "./type.dguu" 
  int stacksize;
  char name[32];
} TASK ;




/* stacks of tasks */

#define STACK_SIZE_TTY		0x8000
#define STACK_SIZE_TESTA	0x8000
#define STACK_SIZE_TESTB	0x8000
#define STACK_SIZE_TESTC	0x8000

#define STACK_SIZE_TOTAL	(STACK_SIZE_TESTA + \
				STACK_SIZE_TESTB + \
				STACK_SIZE_TESTC + \
                                STACK_SIZE_TTY)


#endif

//1658D916
//negiup
