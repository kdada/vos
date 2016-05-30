ENTRY(main)
OUTPUT_FORMAT(elf64-x86-64)

/*物理地址*/
KERNEL_PMA = 0x100000;
/*线性地址*/
KERNEL_LMA = 0xffffffff00000000;
/*页偏移*/
PAGE_ALIGN = 0x1000;
SECTIONS
{
	. = KERNEL_LMA;
	.text KERNEL_LMA : AT(KERNEL_PMA) ALIGN(0x1000)
	{
		*(.text.startup*)
		*(.text)
		*(.text*)
	}
	
	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
		*(.rodata*)
	}
	
	.data : ALIGN(0x1000)
	{
		*(.data)
		*(.data*)
	}
	
	.bss : ALIGN(0x1000)
	{
		*(.bss)
		*(.bss*)
		*(COMMON)
	}
	SECTION_END = .;
	
	
}

/*初始化符号地址*/
/*内核加载地址*/
LinkerKernelLoadAddr = KERNEL_PMA;
/*内核起始虚拟地址*/
LinkerKernelStartVirtualAddr = KERNEL_LMA;
/*内核结束虚拟地址*/
LinkerKernelEndVirtualAddr = SECTION_END;
