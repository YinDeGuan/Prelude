//

#ifndef const_DGUU
#define const_DGUU

#define EXTERN extern
#define	PUBLIC		       // PUBLIC is the opposite of PRIVATE 
#define	PRIVATE	static	       // PRIVATE x limits the scope of x 

#define TRUE 1
#define FALSE 0


#define PRIVILEGE_KERNEL 0
#define PRIVILEGE_TASK 1
#define PRIVILEGE_USER 3

#define PRIVILEGE_USER2 2

#define	RPL_KRNL	SA_RPL0
#define	RPL_TASK	SA_RPL1
#define	RPL_USER	SA_RPL3


#define INT_M_CTL 0x20
#define INT_M_CTLMASK 0x21
#define INT_S_CTL 0xA0
#define INT_S_CTLMASK 0xA1




#define	GDT_SIZE 128
#define IDT_SIZE 256
#define LDT_SIZE 2



/* Hardware interrupts */
#define	NR_IRQ		16	/* Number of IRQs */
#define	CLOCK_IRQ	0
#define	KEYBOARD_IRQ	1
#define	CASCADE_IRQ	2	/* cascade enable for 2nd AT controller */
#define	ETHER_IRQ	3	/* default ethernet interrupt vector */
#define	SECONDARY_IRQ	3	/* RS232 interrupt vector for port 2 */
#define	RS232_IRQ	4	/* RS232 interrupt vector for port 1 */
#define	XT_WINI_IRQ	5	/* xt winchester */
#define	FLOPPY_IRQ	6	/* floppy disk */
#define	PRINTER_IRQ	7
#define	AT_WINI_IRQ	14	/* at winchester */




#define NR_SYS_CALL 2


/* TTY */
#define NR_CONSOLES	3	/* consoles */

/* Number of tasks */
#define NR_TASKS	4


//VGA

#define	V_MEM_BASE	0xB8000	/* base of color video memory */
#define	V_MEM_SIZE	0x8000	/* 32K: B8000H -> BFFFFH */

/* Color */
/*
 * e.g. MAKE_COLOR(BLUE, RED)
 *      MAKE_COLOR(BLACK, RED) | BRIGHT
 *      MAKE_COLOR(BLACK, RED) | BRIGHT | FLASH
 */
#define BLACK   0x0     /* 0000 */
#define WHITE   0x7     /* 0111 */
#define RED     0x4     /* 0100 */
#define GREEN   0x2     /* 0010 */
#define BLUE    0x1     /* 0001 */
#define FLASH   0x80    /* 1000 0000 */
#define BRIGHT  0x08    /* 0000 1000 */
#define MAKE_COLOR(x,y) (x | y) /* MAKE_COLOR(Background,Foreground) */



#define INDEX_FLATC_R3 (INDEX_LDT_FIRST + NR_TASKS + 1 )
#define INDEX_FLATRW_R3 (INDEX_FLATC_R3 + 1 ) 
#define INDEX_GATE_R3TOR0 (INDEX_FLATRW_R3 + 1 ) 




#endif

//0950D616
//negiup
