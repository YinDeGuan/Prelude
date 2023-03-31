;


;下面对描述符的类型 分为 段和门
;但类型为段或门 并不能决定所使用的 描述符数据结构
;好比 TSS 归于门类 但使用 段描述符 而不是门描述符
;

;
;High                                                                        low
;    BYTE7    BYTE6    BYTE5    BYTE4    BYTE3    BYTE2    BYTE1     BYTE0
;   31..24    属性      属性      23  ...  ...   ...  0      15 ...  ... 0
;   段基址2                               段基址1                 段界限1

;          BYTE6                           
;   7    6    5    4       3  2  1  0          
;   G   D/B   0   AVL    段界限2 19...16            

;          BYTE5
;   7    6 5       4       3..0
;   P    DPL       S       TYPE 
;



;
;PM下 段的界限大小为 20 bit  , 0-19
;



;note bracket
%macro Descriptor 3
	dw (%2) & 0ffffh
	dw (%1) & 0ffffh
	db ((%1) >> 16) & 0ffh
	dw ((%2) >> 8) & 0f00h | ((%3) & 0f0ffh)
	db ((%1) >> 24) & 0ffh
%endmacro 	

;stored segment descriptor
DA_32 EQU 4000h		; D/B = 1 for protected mode
DA_C EQU 98h				; bit7P = 1 , bit4S = 1 , bit0~3TYPE = 8 so only executable 
DA_DRWA EQU 93h	; bit7P = 1 , bit4S = 1 , bit0~3TYPE = 3 read or write to already acessed
DA_DRW EQU 92h ;	;bit7P =1 , bit4S = 1 , bit0~3 TYPE = 2 read or write
DA_DR EQU 90h 		;bit7P =1 , bit4S = 1 , bit0~3TYPE = only readonly
DA_CR EQU 9ah 			;bit7P =1 , bit4S = 1, bit0~3TYPE = executable or read
DA_CCO EQU 9ch 		;bit7P=1 , bit4S =1 , bit0~3TYPE = only execute to accord code , 386 call door
DA_CCOR EQU  9eh 	;bit7P=1 , bit4S =1 , bit0~3TYPE = executed or read for accord code , 386 interrupt door

;system segment descriptor 
DA_LDT EQU 82h 				;bit7P =1 so exist memory and bit4S = 0 that it belong to system , and read or write
DA_TASKGATE EQU 85h 		;bit7P=1 bit4S =0 bit0~3 = only read and upward expand and already read so task gate
DA_386TSS EQU 89h 			;as above ,but only executed and already , 可用386TSS
DA_386CGATE EQU 8ch 		;386  call gate
DA_386IGATE EQU 8eh		;as above , but executed or read for accord code that 386 interrupt gate
DA_386TGATE EQU 8fh		;as above , execute or read for accord code and already accesedd , 386 trap gate



DA_DPL0 EQU 00h 	
DA_DPL1 EQU 20h
DA_DPL2 EQU 40h
DA_DPL3 EQU 60h

SA_RPL0 EQU 0
SA_RPL1 EQU 1
SA_RPL2 EQU 2
SA_RPL3 EQU 3

SA_TIG EQU 0
SA_TIL EQU 4

%macro Gate 4
	dw ((%2) & 0ffffh) 
	dw (%1)
	dw ((%3) & 1fh) | ( ( (%4) << 8) & 0ff00h)
	dw (((%2) >> 16) & 0ffffh)
%endmacro

        ; 1 目标选择子
        ; 2 目标偏移量
        ; 3 目标参数
        ; 4 目标属性



DA_LIMIT_4K EQU 8000h	

PG_P EQU 1
PG_RWR EQU 0
PG_RWW EQU 2
PG_USS EQU 0
PG_USU EQU 4


;0756N1103
;negiup
