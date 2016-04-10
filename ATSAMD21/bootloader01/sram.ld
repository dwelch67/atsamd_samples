
MEMORY
{
    ram : ORIGIN = 0x20007000, LENGTH = 0x1000
}

SECTIONS
{
    .text : { *(.text*) } > ram
    .rodata : { *(.rodata*) } > ram
    .bss : { *(.bss*) } > ram
}

