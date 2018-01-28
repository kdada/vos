set arch i386:x86-64:intel
set disassemble-next-line on
set disassembly-flavor intel
file ./build/kernel.img
b main
tui enable

