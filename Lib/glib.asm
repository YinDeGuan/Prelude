;


;-------------------------------------
;
;Lib for C
;
;

GLOBAL _appear4
GLOBAL _str_len
GLOBAL _str_cpy
GLOBAL _str_ins
GLOBAL _str_set
GLOBAL out_byte
GLOBAL in_byte
GLOBAL _feed
GLOBAL _i_to_h
GLOBAL _i_to_d


GLOBAL disable_irq
GLOBAL enable_irq
GLOBAL disable_int
GLOBAL enable_int

%INCLUDE"../GLib/_appear4.asm"
%INCLUDE"../GLib/_str_len.asm"
%INCLUDE"../GLib/_str_cpy.asm"
%INCLUDE"../GLib/_str_ins.asm"
%INCLUDE"../GLib/_str_set.asm"
%INCLUDE"../GLib/_feed.asm"
%INCLUDE"../GLib/_i_to_h.asm"
%INCLUDE"../GLib/_i_to_d.asm"


%INCLUDE "sconst.inc"


;*****************************************
out_byte :
;
;arg
;  [ebp + 8] port
;  [ebp + 12] value
;
;
push ebp
mov ebp , esp 

push edx

mov edx , [ebp + 8]
mov eax , [ebp + 12]

out dx , al
nop
nop

pop edx

mov esp , ebp
pop ebp
ret






;************************************
in_byte :
;
;arg
; [ebp + 8] port
;
;ret 
;  read value 
;
push ebp
mov ebp , esp

push edx

xor eax , eax

mov edx , [ebp + 8]
in al , dx

pop edx

mov esp , ebp
pop ebp
ret







;****************************************************
disable_irq :
;
; disable specify irq
;  this function will close intterupt of hardware , just as cli 
;
;arg
;  ebp + 8 irq
;
;ret 
;    TRUE is a while ago disable
;    FALSE is already disable
;
;
push ebp
mov ebp , esp

push ecx
pushf
cli

mov ecx , [ebp + 8]

mov ah , 1
rol ah , cl
cmp cl , 8
jae .1

in al , INT_M_CTLMASK
test al , ah
jnz .2
or al , ah
out INT_M_CTLMASK , al
mov eax , 1
popf
pop ecx
mov esp , ebp
pop ebp
ret


                                .1 :
in al , INT_S_CTLMASK
test al , ah
jnz .2
or al , ah
out INT_S_CTLMASK , al
mov eax ,1
popf
pop ecx
mov esp , ebp
pop ebp
ret

                                .2 :

xor eax ,eax

popf
pop ecx

mov esp , ebp
pop ebp
ret





;************************************************
enable_irq:
;
;enable specify interrupt number 
;
;arg 
;  ebp + 8 irq
;
;ret void
;
push ebp
mov ebp , esp

push ecx
pushf
cli             ;may be fast finish for follow

mov ecx , [ebp + 8]          ; irq

mov ah , ~1
rol ah , cl                  ; ah = ~(1 << (irq % 8))
cmp cl , 8
jae .1

in al , INT_M_CTLMASK
and al , ah
out INT_M_CTLMASK , al   

popf
pop ecx
mov esp , ebp
pop ebp
ret
                                .1 :
in al , INT_S_CTLMASK
and al , ah
out INT_S_CTLMASK , al  

popf
pop ecx
mov esp , ebp
pop ebp
ret





;****************************************************
disable_int:
;
;arg 
;ret 
;
;
;

cli
ret




;***********************************************************
enable_int:
;
;arg
;ret
;
;

sti
ret




;1330D316
;1642D916
;1256D1616
;1433D2516
;1153JULY212018
;negiup
