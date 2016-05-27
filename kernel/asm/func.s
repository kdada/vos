
section .text

; 向指定端口(port)输出4字节的数据(data)
;void PortOutWD(Word port,DoubleWord data);
global PortOutWD
PortOutWD:
    mov rdx,rdi
    mov rax,rsi
    out dx,eax
    ret
    
    
; 从指定端口(port)获取4字节数据
;DoubleWord PortInWD(Word port);
global PortInWD
PortInWD:
    xor rax,rax
    mov rdx,rdi
    in eax,dx
    ret
