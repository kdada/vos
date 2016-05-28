# vos
  
开发环境:  
Fedora 23  
Linux 4.4.9-300.fc23.x86_64  
  
编译器:  
x86_64-elf-gcc 6.1.0 cross compiler  
  
模拟器:  
qemu 2.4.1  
bochs 2.6.8  

  
gen.sh:  
用于生成Makefile  

```
bochs调试:  
(1)b addr 在物理地址上设置断点  
   lb addr 在线性地址上设置断点  
   pb addr 在物理地址上设置断点  
   watch read addr 设置读断点  
   watch write addr 设置写断点  
(2)info break 显示所有断点信息  
   blist 显示断点状态    
   bpd num 启用断点  
   bpe num 禁用断点  
   d num 删除断点  
   unwatch read addr 清除读断点    
   unwatch write addr 清除写断点   
   watch 显示所有读写断点  
   unwatch 清除所有读写断点  
   watch stop 遇到读写断点停止执行  
   watch continue 遇到读写断点继续执行  
(3)c 一直执行直至遇到断点   
   s 单步执行  
   n 单步执行(跳过函数)  
(4)info program 显示程序执行的情况  
   info registers|reg|r 显示寄存器的信息  
   info pb|pbreak|b|break 相当于blist  
   info dirty 显示脏页的页地址  
   info cpu 显示所有CPU寄存器的值  
   info fpu 显示所有FPU寄存器的值  
   info idt 显示IDT  
   info gdt num 显示GDT  
   info ldt 显示LDT  
   info tss 显示TSS  
   info pic 显示PIC  
   info ivt num num 显示IVT  
   info flags 显示状态寄存器  
   info cr 显示CR系列寄存器  
   info symbols 显示symbol信息  
   info ne2k|ne2000 显示虚拟的ne2k网卡信息  
(5)r 查看寄存器信息  
   sreg 查看段寄存器  
   creg 查看控制寄存器  
(6)x /nuf addr 显示线性地址内容  
   xp /nuf addr 显示物理地址内容  
   n 显示的单元数  
   u 每个显示单元的大小，u可以是下列之一:  
     b BYTE  
     h WORD  
     w DWORD  
     g QUADWORD  
   f 显示格式，f可以是下列之一:  
     x 按照十六进制显示  
     d 十进制显示  
     u 按照无符号十进制显示  
     o 按照八进制显示  
     t 按照二进制显示  
     c 按照字符显示  
(7)u [num] start end 反汇编一段内存  
(8)trace-reg on 每执行一条语句都输出寄存器信息  

qemu调试:  
(1)启动qemu时参数种需要携带 -gdb tcp::port -S  
   其中port表示端口号,－S表示qemu启动后立即等待gdb控制    
(2)打开gdb,输入target remote:port,端口与上面相同,即可连接并开始调试    
   gdb:  
      set arch 可以显示所有可用架构  
      set arch i8086 设置架构为8086,否则在gdb调试实模式时反汇编不正常
      从实模式切换到长模式的时候gdb会出现错误，此时先disconnect.然后设置架构为i386:x86-64:intel再重新连接 
      set disassemble-next-line on 设置执行后自动反汇编下一行并显示  
      set disassembly-flavor intel 设置汇编显示为intel汇编  

   gdb调试:
      b *addr 在地址上设置断点    
      c 继续执行直至遇到断点    
      si 单步执行一条指令    
      ni 单步执行一条指令(跳过函数)  
      delete num 删除断点  
      info reg 查看寄存器  
      finish 执行到返回  
      x/nuf addr  
          u 每个显示单元的大小，u可以是下列之一:  
         b 表示单字节  
         h 表示双字节  
         w 表示四字节  
         g 表示八字节  
         
         f显示格式，f可以是下列之一:  
         x 按十六进制格式显示变量  
         d 按十进制格式显示变量  
         u 按十进制格式显示无符号整型  
         o 按八进制格式显示变量  
         t 按二进制格式显示变量  
         a 按十六进制格式显示变量  
         i 指令地址格式  
         c 按字符格式显示变量  
         s 按字符串格式显示变量  
         f 按浮点数格式显示变量  
         
        
```