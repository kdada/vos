org 0x7c00
Main:
	;读取内存布局信息
	mov di,0x100
	mov es,di
	xor di,di
	xor ebx,ebx
	mov edx,0x534D4150
ReadMM:
	mov eax,0xe820
	mov ecx,24
	int 0x15
	add edi,24
	cmp ebx,0
	jnz ReadMM
	;将下一块全部置为0
	mov eax,0
	mov ecx,6
	rep stosd
	
	
	;加载内核到0x10000
	mov bx,0x1000
	mov es,bx
	xor bx,bx
	mov ax,0x0218
	mov cx,0x000a
	mov dx,0x0000
	int 0x13
SwitchTo64:
	;关中断
	cli
	;打开A20地址线
	in al, 0x92
	or al, 2
	out 0x92, al
	
	;PML4T起始地址
	mov edi,0x8000 
	;设置CR3为PML4T基址
	mov cr3,edi
	
	;构造页表
	xor eax,eax
	mov es,ax
	mov ecx,0x2000;2*4k
	rep stosd;8*4k的区域清0,用于存放PML4T,PDPT,PDT,PT
	
	;构造0x0开始的2M的页表
	mov edi,cr3
	mov dword [edi],0x9003;PML4T第一个表项 
	add edi,0x1000
	mov dword [edi],0xa003;PDPT第一个表项
	add edi,0x1000
	mov dword [edi],0xb003;PDT第一个表项
	add edi,0x1000
	mov ebx,0x3;PAGE_PRESENT(0)=1,PAGE_WRITE(1)=1
	mov ecx,0x200;512
	.GenEntry:;生成前2M区域的映射,共512个表项,每个表项8字节
	mov dword [edi],ebx
	add ebx,0x1000
	add edi,8
	loop .GenEntry
	
	;构造0xffffffff00000000开始的2M的页表
	mov edi,cr3
	mov dword [edi+511*8],0xc002;PML4T最后一个表项 
	mov edi,0xc000
	mov dword [edi+508*8],0xd002;PDPT第一个表项
	mov edi,0xd000
	mov dword [edi],0xe002;PDT第一个表项
	mov edi,0xe000
	mov ebx,0x100003;PAGE_PRESENT(0)=1,PAGE_WRITE(1)=1
	mov ecx,0x200;512
	.GenEntry2:;生成2M区域的映射,共512个表项,每个表项8字节
	mov dword [edi],ebx
	add ebx,0x1000
	add edi,8
	loop .GenEntry2
	
	
	;设置IDT和GDT,GDT的代码段描述符的D/B(22)=0,L(21)＝1(标志64位代码段)
	lidt [IDT.Pointer]
	lgdt [GDT.Pointer]
	;设置CR4的PAE(5)和PGE(7)
	mov eax,cr4
	or eax,0xa0
	mov cr4,eax
	;设置MSR 0xC0000080的LME(8)
	mov ecx,0xc0000080
	rdmsr
	or eax,0x100
	wrmsr
	;设置CR0的PG(31)和PE(0)
	mov eax,cr0
	or eax,0x80000001
	mov cr0,eax
	;使用CS跳转到64位代码段
	jmp 0x8:JumpToKernel
	
bits 64
JumpToKernel:
	;将核心代码移动到0x100000(1M)
	mov ax,0x10
	mov ss,ax
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov rcx,0x600
	mov rsi,0x10000
	mov rdi,0x100000
	rep movsq
	;跳转到核心代码
	mov rdi,0x1000
	mov rbx,0x10000
	call rbx
	hlt

IDT:
	.Pointer:
	dq 0

GDT:
	dq 0
	dq 0x00209A0000000000
	dq 0x0000920000000000
	.Pointer:
	dw $ - GDT - 1
	dd GDT

times 510-($-$$) db 0
db 0x55,0xaa