ENTRY(main)
OUTPUT_FORMAT(elf64-x86-64)

/*物理地址*/
KERNEL_PMA = 0x100000;
/*线性地址*/
KERNEL_LMA = 0xffffffff00000000;

SECTIONS
{
	.text KERNEL_LMA : AT(KERNEL_PMA) ALIGN(4k)
	{
		*(.text.startup*)
		*(.text)
		*(.text*)
	}
	
	.rodata : ALIGN(4k)
	{
		*(.rodata)
		*(.rodata*)
	}
	
	.data : ALIGN(4k)
	{
		*(.data)
		*(.data*)
	}
	
	.bss : ALIGN(4k)
	{
		*(.bss)
		*(.bss*)
	}
	
	.other : ALIGN(4k)
	{
		*(COMMON)
	}
}
