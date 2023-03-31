//

// AT PS/2 键盘
// PS/2 键盘 和 USB键盘 是当今最流行的两种键盘 .
// AT键盘要稍微老一点 ,
//
// AT 即 接口圆头 5引脚 键盘
// PS 即 接口圆头 6引脚 键盘
// USB 即 接口扁头 键盘
//
//
// 在更早的时候 , IBM还推出过 XT键盘 , 它也使用 5-pin DIN ,
// 不过现在不再使用 , 如今主流键盘主要是 AT PS/2 USB 三种 .
// 随后不为 USB 只是为 AT 和 PS/2  
//
//
// 在键盘中存在一枚叫做 键盘编码器 (keyboard Encoder) 的芯片 ,
// 它通常是 Intel 8048 以及 兼容芯片 , 作用是监视键盘的键入 ,
// 并把适当的数据传送给计算机 . 另外 , 在计算机主板上还有一个键盘控制器8042 (keyboard controller) , 
// 用来接收和解码来自键盘的数据 , 并与8259A 以及软件等进行通信 . 
// 
// 敲击键盘有两个方面的含义 : 动作和内容 . 动作可以分解成三类 : 按下 , 保持按住 , 放开 , 
// 内容则是键盘上不同的键 , 字母键 还是数字键 , 回车键还是箭头键 . 
// 所以 , 根据敲击动作产生的编码 , 8048 既要反映 哪个 按键产生了动作 , 还要反映产生了什么动作 .
//
// 敲击键盘所产生的编码被称做扫描码 Scan Code , 它分为Make Code 和 Break Code 两类 .
// 当一个键被按下或者保持按下时 , 将会产生 Make Code , 当键弹起时 , 产生Break code .
// 除了Pause键之外 , 每一个按键都对应一个Make Code 和一个Break Code .
// 
// 扫描码总共有三套 , 叫做 Scan code set1 , Scan code set2 和 Scan code set3 .
// Scan code set1 是早期的XT键盘使用的 , 现在的键盘默认都支持 Scan code set2 ,
// 而 Scan code set3 很少使用 .
// 
// 当8048 检测到一个键的动作后 , 会把相应的扫描码发送给8042 ,  
// 8042会把它转换成相应的Scan code set1 扫描码 , 并将其放置在输入缓冲区中 , 
// 然后8042告诉8259A产生中断 IRQ1.  如果此时键盘又有新的键被按下 , 8042将不再接收 ,
// 一直到缓冲区被清空 , 8042 才会收到更多的扫描码 .
//    
// 8042接收一次 发生一次IRQ1 , 而8042缓冲区只有 一个字节大小 (一个寄存器) .
//  
// 
//             8042 寄存器
//  寄存器名称     寄存器大小    端口    R/W    用法
//  输出缓冲区     1BYTE       0x60    Read    读输出缓冲区
//  输入缓冲区     1BYTE       0x60    Write   写输入缓冲区
//  状态寄存器     1BYTE       0x64    Read    读状态寄存器
//  控制寄存器     1BYTE       0x64    Write   发送命令  
//
//  读输出缓冲区将 清空缓冲区 ...
//
//  这里的输出 或输入 相对于系统而言 ...
//
//
//
// 调整键盘LED灯
//
// 8042输入缓冲区和8042控制寄存器都是可写的 , 但它们的作用是不同的 , 
//  写入8042输入缓冲区用来往8048发送命令 , 而写入8042控制寄存器是往8042本身发送命令
// 
// 向8048发送命令 可以 告知 8048调整LED
//
// 使用端口 0x60 , 设置LED的命令是0xED ,
// 当键盘接收到这个命令后 , 会回复一个ACK(0xFA) , 然后等待从端口 0x60
// 写入的LED参数字节 , 这个参数字节定义
//
//   BIT      BINARAY
//   7        0
//   6        0
//   5        0
//   4        0
//   3        0
//   2        Caps Lock        0 熄灭 , 1 点亮
//   1        Num Lock         0 熄灭 , 1 点亮
//   0        Scroll Lock      0 熄灭 , 1 点亮
//
// 
// 当键盘收到参数字节后 , 会再回复一个ACK , 并根据参数字节的值来设置 LED
//
// 要注意 , 在向 8042输入缓冲区写入数据时 , 要先判断 以下输入缓冲区是否为空 ,
// 方法是通过端口0x64读取状态寄存器 , 状态寄存器的 BIT1 如果为 0 ,
// 表示输入缓冲区是空的 , 可以向其写入数据 
// 
//
// 出于不同的键盘 , 这里免去了PAD部分...
//
//  8042对不同的键盘可能是相同 , 它的一个主要目的就是为了提供相同的键盘接口
//  实际的结果 似乎也是如此 .
//







#include "tosh.h"

/* 
  Keymap for US MF-2 keyboard.  
*/

u32 keymap[NR_SCAN_CODES * MAP_COLS] = {

/* scan-code			!Shift		Shift		E0 XX	*/
/* ==================================================================== */
/* 0x00 - none		*/	0,		0,		0,
/* 0x01 - ESC		*/	ESC,		ESC,		0,
/* 0x02 - '1'		*/	'1',		'!',		0,
/* 0x03 - '2'		*/	'2',		'@',		0,
/* 0x04 - '3'		*/	'3',		'#',		0,
/* 0x05 - '4'		*/	'4',		'$',		0,
/* 0x06 - '5'		*/	'5',		'%',		0,
/* 0x07 - '6'		*/	'6',		'^',		0,
/* 0x08 - '7'		*/	'7',		'&',		0,
/* 0x09 - '8'		*/	'8',		'*',		0,
/* 0x0A - '9'		*/	'9',		'(',		0,
/* 0x0B - '0'		*/	'0',		')',		0,
/* 0x0C - '-'		*/	'-',		'_',		0,
/* 0x0D - '='		*/	'=',		'+',		0,
/* 0x0E - BS		*/	BACKSPACE,	BACKSPACE,	0,
/* 0x0F - TAB		*/	TAB,		TAB,		0,
/* 0x10 - 'q'		*/	'q',		'Q',		0,
/* 0x11 - 'w'		*/	'w',		'W',		0,
/* 0x12 - 'e'		*/	'e',		'E',		0,
/* 0x13 - 'r'		*/	'r',		'R',		0,
/* 0x14 - 't'		*/	't',		'T',		0,
/* 0x15 - 'y'		*/	'y',		'Y',		0,
/* 0x16 - 'u'		*/	'u',		'U',		0,
/* 0x17 - 'i'		*/	'i',		'I',		0,
/* 0x18 - 'o'		*/	'o',		'O',		0,
/* 0x19 - 'p'		*/	'p',		'P',		0,
/* 0x1A - '['		*/	'[',		'{',		0,
/* 0x1B - ']'		*/	']',		'}',		0,
/* 0x1C - CR/LF		*/	ENTER,		ENTER,		PAD_ENTER,
/* 0x1D - l. Ctrl	*/	CTRL_L,		CTRL_L,		CTRL_R,
/* 0x1E - 'a'		*/	'a',		'A',		0,
/* 0x1F - 's'		*/	's',		'S',		0,
/* 0x20 - 'd'		*/	'd',		'D',		0,
/* 0x21 - 'f'		*/	'f',		'F',		0,
/* 0x22 - 'g'		*/	'g',		'G',		0,
/* 0x23 - 'h'		*/	'h',		'H',		0,
/* 0x24 - 'j'		*/	'j',		'J',		0,
/* 0x25 - 'k'		*/	'k',		'K',		0,
/* 0x26 - 'l'		*/	'l',		'L',		0,
/* 0x27 - ';'		*/	';',		':',		0,
/* 0x28 - '\''		*/	'\'',		'"',		0,
/* 0x29 - '`'		*/	'`',		'~',		0,
/* 0x2A - l. SHIFT	*/	SHIFT_L,	SHIFT_L,	0,
/* 0x2B - '\'		*/	'\\',		'|',		0,
/* 0x2C - 'z'		*/	'z',		'Z',		0,
/* 0x2D - 'x'		*/	'x',		'X',		0,
/* 0x2E - 'c'		*/	'c',		'C',		0,
/* 0x2F - 'v'		*/	'v',		'V',		0,
/* 0x30 - 'b'		*/	'b',		'B',		0,
/* 0x31 - 'n'		*/	'n',		'N',		0,
/* 0x32 - 'm'		*/	'm',		'M',		0,
/* 0x33 - ','		*/	',',		'<',		0,
/* 0x34 - '.'		*/	'.',		'>',		0,
/* 0x35 - '/'		*/	'/',		'?',		PAD_SLASH,
/* 0x36 - r. SHIFT	*/	SHIFT_R,	SHIFT_R,	0,
/* 0x37 - '*'		*/	'*',		'*',    	0,
/* 0x38 - ALT		*/	ALT_L,		ALT_L,  	ALT_R,
/* 0x39 - ' '		*/	' ',		' ',		0,
/* 0x3A - CapsLock	*/	CAPS_LOCK,	CAPS_LOCK,	0,
/* 0x3B - F1		*/	F1,		F1,		0,
/* 0x3C - F2		*/	F2,		F2,		0,
/* 0x3D - F3		*/	F3,		F3,		0,
/* 0x3E - F4		*/	F4,		F4,		0,
/* 0x3F - F5		*/	F5,		F5,		0,
/* 0x40 - F6		*/	F6,		F6,		0,
/* 0x41 - F7		*/	F7,		F7,		0,
/* 0x42 - F8		*/	F8,		F8,		0,
/* 0x43 - F9		*/	F9,		F9,		0,
/* 0x44 - F10		*/	F10,		F10,		0,
/* 0x45 - NumLock	*/	NUM_LOCK,	NUM_LOCK,	0,
/* 0x46 - ScrLock	*/	SCROLL_LOCK,	SCROLL_LOCK,	0,
/* 0x47 - Home		*/	PAD_HOME,	'7',		HOME,
/* 0x48 - CurUp		*/	PAD_UP,		'8',		UP,
/* 0x49 - PgUp		*/	PAD_PAGEUP,	'9',		PAGEUP,
/* 0x4A - '-'		*/	PAD_MINUS,	'-',		0,
/* 0x4B - Left		*/	PAD_LEFT,	'4',		LEFT,
/* 0x4C - MID		*/	PAD_MID,	'5',		0,
/* 0x4D - Right		*/	PAD_RIGHT,	'6',		RIGHT,
/* 0x4E - '+'		*/	PAD_PLUS,	'+',		0,
/* 0x4F - End		*/	PAD_END,	'1',		END,
/* 0x50 - Down		*/	PAD_DOWN,	'2',		DOWN,
/* 0x51 - PgDown	*/	PAD_PAGEDOWN,	'3',		PAGEDOWN,
/* 0x52 - Insert	*/	PAD_INS,	'0',		INSERT,
/* 0x53 - Delete	*/	PAD_DOT,	'.',		DELETE,
/* 0x54 - Enter		*/	0,		0,		0,
/* 0x55 - ???		*/	0,		0,		0,
/* 0x56 - ???		*/	0,		0,		0,
/* 0x57 - F11		*/	F11,		F11,		0,	
/* 0x58 - F12		*/	F12,		F12,		0,	
/* 0x59 - ???		*/	0,		0,		0,	
/* 0x5A - ???		*/	0,		0,		0,	
/* 0x5B - ???		*/	0,		0,		GUI_L,	
/* 0x5C - ???		*/	0,		0,		GUI_R,	
/* 0x5D - ???		*/	0,		0,		APPS,	
/* 0x5E - ???		*/	0,		0,		0,	
/* 0x5F - ???		*/	0,		0,		0,
/* 0x60 - ???		*/	0,		0,		0,
/* 0x61 - ???		*/	0,		0,		0,	
/* 0x62 - ???		*/	0,		0,		0,	
/* 0x63 - ???		*/	0,		0,		0,	
/* 0x64 - ???		*/	0,		0,		0,	
/* 0x65 - ???		*/	0,		0,		0,	
/* 0x66 - ???		*/	0,		0,		0,	
/* 0x67 - ???		*/	0,		0,		0,	
/* 0x68 - ???		*/	0,		0,		0,	
/* 0x69 - ???		*/	0,		0,		0,	
/* 0x6A - ???		*/	0,		0,		0,	
/* 0x6B - ???		*/	0,		0,		0,	
/* 0x6C - ???		*/	0,		0,		0,	
/* 0x6D - ???		*/	0,		0,		0,	
/* 0x6E - ???		*/	0,		0,		0,	
/* 0x6F - ???		*/	0,		0,		0,	
/* 0x70 - ???		*/	0,		0,		0,	
/* 0x71 - ???		*/	0,		0,		0,	
/* 0x72 - ???		*/	0,		0,		0,	
/* 0x73 - ???		*/	0,		0,		0,	
/* 0x74 - ???		*/	0,		0,		0,	
/* 0x75 - ???		*/	0,		0,		0,	
/* 0x76 - ???		*/	0,		0,		0,	
/* 0x77 - ???		*/	0,		0,		0,	
/* 0x78 - ???		*/	0,		0,		0,	
/* 0x78 - ???		*/	0,		0,		0,	
/* 0x7A - ???		*/	0,		0,		0,	
/* 0x7B - ???		*/	0,		0,		0,	
/* 0x7C - ???		*/	0,		0,		0,	
/* 0x7D - ???		*/	0,		0,		0,	
/* 0x7E - ???		*/	0,		0,		0,
/* 0x7F - ???		*/	0,		0,		0
};






PRIVATE	int	code_with_E0;
PRIVATE	int	shift_l;	/* l shift state */
PRIVATE	int	shift_r;	/* r shift state */
PRIVATE	int	alt_l;		/* l alt state	 */
PRIVATE	int	alt_r;		/* r left state	 */
PRIVATE	int	ctrl_l;		/* l ctrl state	 */
PRIVATE	int	ctrl_r;		/* l ctrl state	 */
PRIVATE	int	caps_lock;	/* Caps Lock	 */
PRIVATE	int	num_lock;	/* Num Lock	 */
PRIVATE	int	scroll_lock;	/* Scroll Lock	 */
PRIVATE	int	column ;

PRIVATE u8 get_byte_from_kbuf() ;
PRIVATE void set_leds();
PRIVATE void kb_wait();
PRIVATE void kb_ack();


PRIVATE KB_INPUT kb_in ;    //../Include/keymap.dguu

  // kb_in 在r0下写入 , 在r1下读取 ... 

/* AT keyboard */
/* 8042 ports */
#define KB_DATA		0x60	/* I/O port for keyboard data
					Read : Read Output Buffer
					Write: Write Input Buffer(8042 Data&8048 Command) */
#define KB_CMD		0x64	/* I/O port for keyboard command
					Read : Read Status Register
					Write: Write Input Buffer(8042 Command) */



#define LED_CODE	0xED

#define KB_ACK		0xFA





/*
  好比 当PasueBreak按下会触发6次键盘中断 , 
   每触发一次 需要读一次 8042的KB_DATA端口 , 才能再次接收触发 . 直到6次触发完毕 . 
  
*/
PUBLIC void keyboard_handler(int irq)
{
  u8 scan_code = in_byte(KB_DATA);

  if(kb_in.count < KB_IN_BYTES) 
  {
    *(kb_in.p_head) = scan_code;

    kb_in.p_head++;
   
    if(kb_in.p_head == kb_in.buf + KB_IN_BYTES)
      kb_in.p_head = kb_in.buf;

    kb_in.count++;
  }

}





// key board interrupt handled after init keyboard .
// that's good logic.

PUBLIC void init_keyboard()
{
  kb_in.count = 0 ;
  kb_in.p_head = kb_in.p_tail = kb_in.buf ;

//  _feed( &disp_pos) ;
//  _appear4( &disp_pos , "static value : %d " , (void*)shift_l );  
            //verdict static value 
// even if static variable isn't zero in ELF for initial?
//

  shift_l = shift_r = 0;
  alt_l	= alt_r = 0;
  ctrl_l = ctrl_r = 0;

  caps_lock = 0 ;
  num_lock = 1 ;
  scroll_lock = 0 ;
  
  set_leds() ;
  


  put_irq_handler(KEYBOARD_IRQ , keyboard_handler) ;
  enable_irq(KEYBOARD_IRQ) ;                       

}





PUBLIC void keyboard_read(TTY* p_tty)
{
  u8 scan_code;
  char output[2];
  int make ;	/* 1: make;  0: break. */

  u32 key = 0 ;/* 用一个整型来表示一个键。比如，如果 Home 被按下，
		 * 则 key 值将为 'HOME'。
		 */

  u32* keyrow ;	/* 指向 keymap[] 的某一行 */


  int i;
  u8 pausebrk_scode[] = {0xE1, 0x1D, 0x45,
		       0xE1, 0x9D, 0xC5};
  int is_pausebreak = 1;


  if(kb_in.count > 0)
  {
    code_with_E0 = 0;

    scan_code = get_byte_from_kbuf();


    
    if(scan_code == 0xE1) 
    {

      for(i=1;i<6;i++)
      {
        if(get_byte_from_kbuf() != pausebrk_scode[i]) 
        {
	  is_pausebreak = 0;
	  break;
	}
      }
    
      if(is_pausebreak) 
      {
        key = PAUSEBREAK;
      }
    }

    
    
    
    else if(scan_code == 0xE0)
    {
      scan_code = get_byte_from_kbuf();

	/* PrintScreen 被按下 */
      if(scan_code == 0x2A) 
      {
        if(get_byte_from_kbuf() == 0xE0) 
        {
	  if(get_byte_from_kbuf() == 0x37)
          {
            key = PRINTSCREEN;
	    make = 1;
	  }
	}
      }

      /* PrintScreen 被释放 */
      if(scan_code == 0xB7) 
      {
        if(get_byte_from_kbuf() == 0xE0)
        {
          if(get_byte_from_kbuf() == 0xAA)
          {
	    key = PRINTSCREEN;
	    make = 0;
	  }
	}
      }

	/* 不是PrintScreen, 此时scan_code为0xE0紧跟的那个值. */
      if(key == 0)          
      {
	code_with_E0 = 1;    
      }
    }


    if( ( key != PAUSEBREAK) && (key != PRINTSCREEN ) ) 
    {
      /* 首先判断Make Code 还是 Break Code */
      make = (scan_code & FLAG_BREAK ? 0 : 1);

	/* 先定位到 keymap 中的行 */
      keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];
			
      column = 0;

      if(shift_l || shift_r) 
      {
        column = 1;
      }

      if(code_with_E0)
      {
         column = 2; 
	 code_with_E0 = 0;
      }
			
      key = keyrow[column];
			
      switch(key)
      {
        case SHIFT_L :
          shift_l = make ;
	  break;
	
        case SHIFT_R :
	  shift_r = make;
	  break;
	
        case CTRL_L :
	  ctrl_l = make;
	  break;
	
	case CTRL_R :
	  ctrl_r = make;
	  break;
	
        case ALT_L:
	  alt_l = make;
	  break;
	
        case ALT_R:
	  alt_l = make;
	  break;
	
        default:
	  break;
      }

      if(make)
      { /* 忽略 Break Code */
        
        key |= shift_l ? FLAG_SHIFT_L : 0 ;
	key |= shift_r ? FLAG_SHIFT_R : 0 ;
	key |= ctrl_l ? FLAG_CTRL_L : 0 ;
	key |= ctrl_r ? FLAG_CTRL_R : 0 ;
	key |= alt_l ? FLAG_ALT_L : 0 ;
	key |= alt_r ? FLAG_ALT_R : 0 ;
			
	in_process(p_tty , key);
      }
    }
  }
}






PRIVATE u8 get_byte_from_kbuf()       /* 从键盘缓冲区中读取下一个字节 */
{
  u8 scan_code;

  while (kb_in.count <= 0) {}   /* 等待下一个字节到来 */

  disable_int();
  scan_code = *(kb_in.p_tail);
  kb_in.p_tail++;
  if(kb_in.p_tail == kb_in.buf + KB_IN_BYTES) 
  {
    kb_in.p_tail = kb_in.buf;
  }
  
  kb_in.count-- ;
  enable_int() ;

  return scan_code ;
}




PRIVATE void kb_wait()	/* 等待 8042 的输入缓冲区空 */
{
  u8 kb_stat;

  do
  {
    kb_stat = in_byte(KB_CMD);    //KB_CMD = 0x64 , 8042状态寄存器 
  }while(kb_stat & 0x02);

}



PRIVATE void kb_ack()
{
  u8 kb_read;

  do 
  {
    kb_read = in_byte(KB_DATA);
  }while(kb_read = ! KB_ACK );   //这只会执行一次 , 或许这是对速度信任 . 

}

//
//8042 寄存器表 看 orange_os_265 
// 键盘led操作方法 看 orange_os_303
//
// 8042 位于 8048 和 计算机 之间 . 
// 键盘led灯的控制需要操作8048 
// 所以要往 输入缓冲区写入  . 
// 
// 8042输入缓冲区 是 计算机 与 8048 沟通的地方 , 这个缓冲区 可被读可被写 . 
//
// 对8042输入缓冲区的读取同时也意味着 清空 . 这样 8042才会再次接受由8048发送的数据 . 
// 


PRIVATE void set_leds()
{

  u8 leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock ;

  kb_wait();
  out_byte(KB_DATA, LED_CODE) ;        //KB_DATA = 0x60 , 8042输入缓冲区
  kb_ack();  

  kb_wait();
  out_byte(KB_DATA, leds) ;
  kb_ack();

}








//1538D2516
//2258D3016
//negiup
