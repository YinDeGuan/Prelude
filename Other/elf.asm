//

//
//随后结构中的偏移量 都是基于 elf文件被载入内存时的地址 .
//也就是把整个elf文件映射到内存时 的 首地址 .
//


//
//	ELF header  
//
//#define EI_NIDENT 16	
//
//   typedef struct 
//   {
//unsigned char e_ident[ EI_NIDENT ];
//Elf32_Half e_type;
//Elf32_Half e_machine;
//Elf32_word e_version;
//Elf32_Addr e_entry;
//Elf32_Off e_phoff;
//Elf32_Off e_shoff;
//Elf32_Word e_flags;
//Elf32_Half e_ehsize;
//Elf32_Half e_phentsize;
//Elf32_Half e_phnum;
//Elf32_Half e_shentsize;
//Elf32_Half e_shnum;
//Elf32_Half e_shstrndx;
//    }Elf32_Ehdr
//
//

//由于ELF文件力求支持8bit到32bit不同架构的处理器 所以才定义了如下的数据
//从而让文件格式与机器无关
//	
//name              size        align          purpose
//Elf32_Addr        4		4		无符号程序地址
//Elf32_Half        2           2               无符号中等大小整数
//Elf32_Off         4	        4		无符号文件偏移
//Elf32_Sword       4		4               有符号大整数
//Elf32_word        4           4               无符号大整数
//unsigned char     1           1               无符号小整数
//		
//最开头是16字节的e_ident 其中包含用以表示ELF文件的字符 以及其他一些与机器无关的信息
//开头的4个字节是固定不变的 第1个字节值 为 0x7f 
//紧跟着就是ELF三个字符      这4字节表明 这个文件是个ELF文件
//	
//e_type 它标识的是该文件的类型  若是2 表示可执行文件
//e_machine 若值是3 表明运行该程序需要的体系结构为Intel80386
//e_version 这个成员确定文件的版本   
//e_entry 程序的入口地址 
//
//e_phoff Program header table 在文件中的偏移量 以字节计数 这里的值是0x34
//0x34是该ELF头的大小
//程序头表中的项是连续的 . 
//
//e_shoff Section header table 在文件中的偏移量 以字节计数 这里的值是 0x1c0
//e_flags 对IA32而言 此项为0
//
//
//e_ehsize ELF header 大小 以字节计数 这里值为0x34 , 定值
/
//e_phentsize Program header table 中一个条目即一个Program header 的大小 0x20 , 定值
//
//
//e_phnum Program header table 中有多少条目
//e_shentszie Sectino header table 中每一个条目即一个 Section header  的大小 
//e_shnum Section header table 中有多少个条目
//e_shstrndx 包含节名称的 字符串表是第几个节 , 从0开始 .
//	
//一般 可能 Program header table 在文件中的偏移量 是 0x34 正如 e_phoff为0x34
//而ELF header 大小 也是0x34 可见ELF header 后面紧接着就是Program header table 
//



//
//程序头项 所指的段 是相对于ELF文件 , 这种段 规划了 ELF 	也就是ELF由多个段构成 .
//每个段可以在内存中对齐摆放 . 而这种段也不是源文件中的code 段 或是 data 段 .
//这种段明显包含了好比像 ELF 的 elf头 以及 program header table 等信息
//而也许程序头中所指其在内存中的内存 也将相对于 代码段的入口地址 , 即 将受到 ld -Ttext address 的约束
//
//整个ELF文件被 program header table 规划为不同的段 , 以及这些段在内存中的布局 .
//这些段在文件中的格式 似乎与在内存中的格式将一致 除了首地址 .
//
//也就是 程序在内存如何 将是被文件格式决定 .
//Linux os 解释器 将根据ELF 解释 可执行文件 . 
//
//
//如下 是 Program header table	
//
//      typedef struct 
//	{
//	Elf32_word		p_type;
//	Elf32_Off			p_offset;
//	Elf32_Addr		P_vaddr;
//	Elf32_Addr		p_paddr;
//	Elf32_Word		p_filesz;
//	Elf32_Word		p_memsz;
//	Elf32_Word		p_flags;
//	Elf32_Word		p_align;
//	}Elf32_Phdr;
//
//	实际上Program header 描述的是系统准备程序运行所需的一个段 Segment
//	或其他信息 
//	p_type 当前Program header 所描述的段的类型   , 该值为 0 则 此程序项无效.
//	p_offset 段的第一个字节在文件中的偏移
//	p_vaddr 段的第一个字节在内存中的虚拟地址
//	p_paddr 在物理地址定位相关的系统中 此项是为物理地址保留
//	p_filesz 段在文件中的长度
//	p_memsz 段在内存中的长度
//	p_flags 与段相关的标志
//	p_align 根据此项值来确定段在文件以及内存中如何对齐
//	Program header 描述的是一个段在文件中的位置 大小以及它被放进内存后
//	所在的位置和大小 
//	如果想把一个文件加载进内存的话 需要的正是这些信息 
//			
//


//1003D216
//2222D416
//negiup
