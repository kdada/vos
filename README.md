# vos
  
开发环境:  
`Fedora 26`  
`Linux 4.11.8-300.fc26.x86_64`  
  
编译器:  
`x86_64-elf-gcc 7.2.0 cross compiler`  

调试器:
`GNU gdb (GDB) Fedora 8.0.1-33.fc26`  
  
模拟器:  
`qemu-system-x86_64 2.9.1`  
  
gen.sh:  
用于生成Makefile，之后可以使用 `make vos` 构建启动镜像

qemu调试:  
(1) 启动 qemu 进行调试  
   `qemu-system-x86_64 -drive format=raw,file=./build/vos.img -s -S`  
   - -s：表示进入 gdb 调试模式，默认监听 1234 端口。  
   - -S：表示 qemu 启动后立即等待 gdb 控制。  

(2) 打开 gdb，输入 `target remote:1234`，端口与上面相同，即可连接并开始调试    
   - gdb:  
     - set arch 可以显示所有可用架构  
     - set arch i8086 设置架构为8086,否则在gdb调试实模式时反汇编不正常
     - 从实模式切换到长模式的时候gdb会出现错误，此时先disconnect.然后设置架构为i386:x86-64:intel再重新连接 
     - 在使用 i8086 时，可能会出现 `Remote 'g' packet reply is too long` 的错误，出现这个错误的时候，直接
     - `set arch i386:x86-64:intel`，然后再次连接（此时反汇编显示不正常，但是可以调试）。
     - set disassemble-next-line on 设置执行后自动反汇编下一行并显示  
     - set disassembly-flavor intel 设置汇编显示为intel汇编  

   - gdb调试:
     - b *addr 在地址上设置断点    
     - c 继续执行直至遇到断点    
     - si 单步执行一条指令    
     - ni 单步执行一条指令(跳过函数)  
     - delete num 删除断点  
     - info reg 查看寄存器  
     - finish 执行到返回  
     - x/nuf addr  
```
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
        
