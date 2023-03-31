;

ORG  0x7C00

;jmp off_address
;如果 off_address < 0x80 为 jmp short off_address
;即 2个字节
; 否则 0x80 < off_address < 0xFFFF 为 jmp off_addess
;即 3个字节 . 
;
;缺省的 编译器 会根据 off_address的大小来判断 2个字节的jmp 还是 3个字节的jmp
;_FAT12H 需要的 是 2个字节的 jmp  .
;
 

;jmp start in _FAT12H.asm
%INCLUDE "/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_FAT12H.asm"

%INCLUDE "/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/Prelude/Boot/LC.asm"
;一些路径默认命令 如 ./ ../ 将相对于 执行nasm时的目录 .
;nasm的%include 实现了./ ../  来与linux对应.
;


szFilename DB "LOADER  BIN" , 0


                                start :

mov ax , cs
mov ds , ax
mov es , ax
mov ss , ax
mov fs , ax
mov gs , ax
mov sp , 0x7C00

push szFilename
push BASEOFLOADERFILE
push OFFSETOFLOADERFILE
call _loadFile
add sp , 6

jmp BASEOFLOADERFILE : OFFSETOFLOADERFILE


%INCLUDE"/thirsty/tryStrive/deduce/hrGx/bottoX/RD3/GLib/_loadFile.asm"



TIMES 510 - ($ - $$)  DB 0
endFlag DW 0xAA55

;1437D416
;negiup
