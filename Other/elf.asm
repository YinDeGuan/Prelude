//

//
//���ṹ�е�ƫ���� ���ǻ��� elf�ļ��������ڴ�ʱ�ĵ�ַ .
//Ҳ���ǰ�����elf�ļ�ӳ�䵽�ڴ�ʱ �� �׵�ַ .
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

//����ELF�ļ�����֧��8bit��32bit��ͬ�ܹ��Ĵ����� ���ԲŶ��������µ�����
//�Ӷ����ļ���ʽ������޹�
//	
//name              size        align          purpose
//Elf32_Addr        4		4		�޷��ų����ַ
//Elf32_Half        2           2               �޷����еȴ�С����
//Elf32_Off         4	        4		�޷����ļ�ƫ��
//Elf32_Sword       4		4               �з��Ŵ�����
//Elf32_word        4           4               �޷��Ŵ�����
//unsigned char     1           1               �޷���С����
//		
//�ͷ��16�ֽڵ�e_ident ���а������Ա�ʾELF�ļ����ַ� �Լ�����һЩ������޹ص���Ϣ
//��ͷ��4���ֽ��ǹ̶������ ��1���ֽ�ֵ Ϊ 0x7f 
//�����ž���ELF�����ַ�      ��4�ֽڱ��� ����ļ��Ǹ�ELF�ļ�
//	
//e_type ����ʶ���Ǹ��ļ�������  ����2 ��ʾ��ִ���ļ�
//e_machine ��ֵ��3 �������иó�����Ҫ����ϵ�ṹΪIntel80386
//e_version �����Աȷ���ļ��İ汾   
//e_entry �������ڵ�ַ 
//
//e_phoff Program header table ���ļ��е�ƫ���� ���ֽڼ��� �����ֵ��0x34
//0x34�Ǹ�ELFͷ�Ĵ�С
//����ͷ���е����������� . 
//
//e_shoff Section header table ���ļ��е�ƫ���� ���ֽڼ��� �����ֵ�� 0x1c0
//e_flags ��IA32���� ����Ϊ0
//
//
//e_ehsize ELF header ��С ���ֽڼ��� ����ֵΪ0x34 , ��ֵ
/
//e_phentsize Program header table ��һ����Ŀ��һ��Program header �Ĵ�С 0x20 , ��ֵ
//
//
//e_phnum Program header table ���ж�����Ŀ
//e_shentszie Sectino header table ��ÿһ����Ŀ��һ�� Section header  �Ĵ�С 
//e_shnum Section header table ���ж��ٸ���Ŀ
//e_shstrndx ���������Ƶ� �ַ������ǵڼ����� , ��0��ʼ .
//	
//һ�� ���� Program header table ���ļ��е�ƫ���� �� 0x34 ���� e_phoffΪ0x34
//��ELF header ��С Ҳ��0x34 �ɼ�ELF header ��������ž���Program header table 
//



//
//����ͷ�� ��ָ�Ķ� �������ELF�ļ� , ���ֶ� �滮�� ELF 	Ҳ����ELF�ɶ���ι��� .
//ÿ���ο������ڴ��ж���ڷ� . �����ֶ�Ҳ����Դ�ļ��е�code �� ���� data �� .
//���ֶ����԰����˺ñ��� ELF �� elfͷ �Լ� program header table ����Ϣ
//��Ҳ������ͷ����ָ�����ڴ��е��ڴ� Ҳ������� ����ε���ڵ�ַ , �� ���ܵ� ld -Ttext address ��Լ��
//
//����ELF�ļ��� program header table �滮Ϊ��ͬ�Ķ� , �Լ���Щ�����ڴ��еĲ��� .
//��Щ�����ļ��еĸ�ʽ �ƺ������ڴ��еĸ�ʽ��һ�� �����׵�ַ .
//
//Ҳ���� �������ڴ���� ���Ǳ��ļ���ʽ���� .
//Linux os ������ ������ELF ���� ��ִ���ļ� . 
//
//
//���� �� Program header table	
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
//	ʵ����Program header ��������ϵͳ׼���������������һ���� Segment
//	��������Ϣ 
//	p_type ��ǰProgram header �������Ķε�����   , ��ֵΪ 0 �� �˳�������Ч.
//	p_offset �εĵ�һ���ֽ����ļ��е�ƫ��
//	p_vaddr �εĵ�һ���ֽ����ڴ��е������ַ
//	p_paddr ��������ַ��λ��ص�ϵͳ�� ������Ϊ������ַ����
//	p_filesz �����ļ��еĳ���
//	p_memsz �����ڴ��еĳ���
//	p_flags �����صı�־
//	p_align ���ݴ���ֵ��ȷ�������ļ��Լ��ڴ�����ζ���
//	Program header ��������һ�������ļ��е�λ�� ��С�Լ������Ž��ڴ��
//	���ڵ�λ�úʹ�С 
//	������һ���ļ����ؽ��ڴ�Ļ� ��Ҫ��������Щ��Ϣ 
//			
//


//1003D216
//2222D416
//negiup