//

//
//开机时 , 看到默认视屏模式 应该是 80* 25 文本模式 .
//在这种模式下 , 显存大小为 32 KB , 占用的范围为 0xB8000 - 0xBFFFF
//每2字节代表一个字符 , 其中低字节表示字符的ASCII码 , 
//高字节表示字符的属性 . 一个屏幕总共可以显示 25 行 , 每行 80个 字节 
//
//
// 默认情况下 屏幕每一个字符对应的 2 字节的定义 
//
//背景色
//   7          高亮          H
//   6          R
//   5          G
//   4          B
//前景色
//   3          高亮
//   2          R
//   1          G
//   0          B               
//ASCII
//   7
//   6
//   5 
//   4          
//   3
//   2
//   1
//   0                       L
//
//
//
//      B/I     R       G       B       进制    作为背景      作为前景
//      0       0       0       0       0h      黑色          黑色
//      0       0       0       1       1h      蓝色          蓝色
//      0       0       1       0       2h      绿色          绿色
//      0       0       1       1       3h      青色          青色
//      0       1       0       0       4h      红色          红色
//      0       1       0       1       5h      洋红          洋红
//      0       1       1       0       6h      棕色          棕色
//      0       1       1       1       7h      白色          白色
//      1       0       0       0       8h      黑色(闪烁)    灰色
//      1       0       0       1       9h      蓝色(闪烁)    亮蓝
//      1       0       1       0       Ah      绿色(闪烁)    亮绿
//      1       0       1       1       Bh      青色(闪烁)    亮青
//      1       1       0       0       Ch      红色(闪烁)    亮红
//      1       1       0       1       Dh      洋红(闪烁)    亮紫
//      1       1       1       0       Eh      棕色(闪烁)    黄色
//      1       1       1       1       Fh      白色(闪烁)    亮白
//
//  也许可以组合 .
//  
// 所以在此模式 一个屏幕所需的内存最大 80 * 25 * 2 = 4000 = 4 Kb
//
// 随后假定 使用 VGA 以上的视屏子系统 , 并假定不使用单色模式 .
// VGA早在 1987 年就被推出了 , 所以这样的假设不会有什么问题 .
//
//
//
// VGA寄存器
//            寄存器                             读端口         写端口
// General Registers            
//         Miscellaneous output Register        0x3CC         0x3C2
//         Input Status Register 0              0x3C2
//         Input Status Register 1              0x3DA
//         Feature Control Register             0x3CA         0x3DA
//         Video Subsystem Enable Register      0x3C3         0x3C3
// 
// Sequencer Registers
//         Address Register                     0x3C4         0x3C4
//         Data    Register                     0x3C5         0x3C5
//
//
//
//            
// CRT Contorller Register
//         Address Register                     0x3D4         0x3D4
//         Data    Register                     0x3D5         0x3D5
//   CRT Controller DATA Registers
//   寄存器名称                                 索引
//   Horizontal Total Register                 00h
//   End Horizontal Display Register           01h
//   Start Horizontal Blanking Register        02h
//   End Horizontal Blanking Register          03h
//   Start Horizontal Retrace Register         04h
//   End Horizontal Retrace Register           05h
//   Vertical Total Register                   06h
//   Overflow Register                         07h
//   Preset Row Scan Register                  08h
//   Maximum Scan Line Register                09h
//   Cursor Start Register                     0Ah
//   Cursor End Register                       0Bh
//   Start Address High Register               0Ch
//   Start Address Low Register                0Dh
//   Cursor Location High Register             0Eh
//   Cursor Location Low Register              0Fh
//   Vertical Retrace Start Register           10h 
//   Vertical Retrace End Register             11h
//   Vertical Display End Register             12h
//   Offset Register                           13h
//   Underline Location Register               14h
//   Start Vertical Blanking Register          15h
//   End Vertical Blanking                     16h
//   CRTC Mode Control Register                17h
//   Line Compare Register                     18h
//
//   需要访问其中一个 CRT Data register 只需要先向 Address Register
//    写对应的索引值(地址寄存器只有一个 , 通过端口 0x3D4) 
//   然后再通过端口 0x3D5 进行操作就是针对索引值对应的寄存器.
//   把 Data Registers 看作一个数组 , Address Register 则像是下标 .
//
//   光标寄存器的计数 不考虑 字符槽属性 . 即 屏幕第1行第2个字符 的索引 为 1 
//   
//   CRT Data register 组中的 start address 寄存器组 设置屏幕显示首地址  参数这是一个偏移量 , 将总是基于0xB8000 ,
//   参数偏移量不考虑字符曹属性 因为 80 * 1 相当于 内存映射起始地址 + 160 .
//   屏幕显示缓冲区 不同于 0xB8000 0xBFFFF 这段 , 也许是一个与显示器相关的一个芯片 , 它映射 0xB8000开始的 4KB 内存
//   到 这个缓冲区 而这个 start address 数据寄存器组可以改变这个 起始地址 如 0xB80A0 
//   在特定模式 .
//   
//   
//
//
//
//
// Graphics Controller Registers
//         Address Register                     0x3CE         0x3CE
//         Data Register                        0x3CF         0x3CF
//
// Attribute Controller Registers
//         Address Register                     0x3C0         0x3C0
//         Data Register                        0x3C1         0x3C0
// 
// Video DAC Palette Registers
//         Write Address                        0x3C8         0x3C8
//         Read Address                                       0x3C7
//         DAC state                            0x3C7
//         Data                                 0x3C9         0x3C9
//         Pel Mask                             0x3C6
//                   
//
//                   
//     




#include "tosh.h"

#define	CRTC_ADDR_REG	0x3D4	/* CRT Controller Registers - Addr Register */
#define	CRTC_DATA_REG	0x3D5	/* CRT Controller Registers - Data Register */
#define	START_ADDR_H	0xC	/* reg index of video mem start addr (MSB) */
#define	START_ADDR_L	0xD	/* reg index of video mem start addr (LSB) */
#define	CURSOR_H	0xE	/* reg index of cursor position (MSB) */
#define	CURSOR_L	0xF	/* reg index of cursor position (LSB) */
#define	V_MEM_BASE	0xB8000	/* base of color video memory */
#define	V_MEM_SIZE	0x8000	/* 32K: B8000H -> BFFFFH */





void update_cursor(int position)
{
  disable_int();       //对端口操作 的 一系列 应该连续 
                       //不排除 这样导致的 速度优化 .

  out_byte(CRTC_ADDR_REG , CURSOR_H );
  out_byte(CRTC_DATA_REG ,  ( (position ) >> 8 ) & 0xFF ) ;
  out_byte(CRTC_ADDR_REG , CURSOR_L );
  out_byte(CRTC_DATA_REG , ( (position) & 0xFF )) ; 

  enable_int() ;
}




void scroll(int line)
{

  disable_int() ;

  out_byte(CRTC_ADDR_REG , START_ADDR_H) ;
  out_byte(CRTC_DATA_REG , ( ( 80 * line ) >> 8 ) & 0xFF );
  out_byte(CRTC_ADDR_REG , START_ADDR_L );
  out_byte(CRTC_DATA_REG , ( 80 * line) & 0xFF );
  
  enable_int();

}



void set_video_start_addr(u32 addr)
{
  disable_int();

  out_byte(CRTC_ADDR_REG, START_ADDR_H) ;
  out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF) ;
  out_byte(CRTC_ADDR_REG, START_ADDR_L) ;
  out_byte(CRTC_DATA_REG, addr & 0xFF) ;

  enable_int();
}


//0836J116
//negiup
