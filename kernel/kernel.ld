OUTPUT_FORMAT(elf-x86-64)
ENTRY(_start)

SECTIONS
{
    . = 0x200000;
    _KernelStart = .;
    .text : ALIGN(0x1000)
    {
        *(.text)
    }
    .data : ALIGN(0x1000)
    {
        *(.data)
    }
    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }
    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }
    _KernelEnd = .;
}