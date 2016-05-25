ENTRY(main)

SECTIONS
{
	.text 0x10000: ALIGN(4K)
	{
		*(.text.startup)
		*(.text)
	}
	.rodata : ALIGN(4K)
	{
		*(.rodata*)
	}
	.data : ALIGN(4K)
	{
		*(COMMON)
		*(.*)
	}
}
