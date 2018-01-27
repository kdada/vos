
section .text

; 向指定端口(port)输出4字节的数据(data)
; void PortOutWD(Word port,DoubleWord data);
global PortOutWD
PortOutWD:
    mov rdx,rdi
    mov rax,rsi
    out dx,eax
    ret
    
    
; 从指定端口(port)获取4字节数据
; DoubleWord PortInWD(Word port);
global PortInWD
PortInWD:
    xor rax,rax
    mov rdx,rdi
    in eax,dx
    ret

; 设置 GDTR
; void LoadGDTRegister(Byte *addr);
global LoadGDTRegister
LoadGDTRegister:
    lgdt [rdi]
    ret

; 设置 CS
; void SetCS(Word cs);
global SetCS
SetCS:
    push rdi
    lea rdi, [rel .Ret]
    push rdi
    retf 
.Ret:
    retn

; 设置 SS
; void SetSS(Word ss);
global SetSS
SetSS:
    mov ss,di

; 设置 DS
; void SetDS(Word ds);
global SetDS
SetDS:
    mov ds,di

; 设置 ES
; void SetES(Word es);
global SetES
SetES:
    mov es,di

; 设置 FS
; void SetFS(Word fs);
global SetFS
SetFS:
    mov fs,di

; 设置 GS
; void SetGS(Word gs);
global SetGS
SetGS:
    mov gs,di


