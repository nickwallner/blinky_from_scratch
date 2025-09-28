/* define execution entry point */
ENTRY(Reset_Handler);

/* define two sections of memory, flash and sram */
MEMORY {
    flash (rx)  : ORIGIN = 0x08000000, LENGTH = 128M
    sram  (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
}

/* create and define symbol _estack whose value is the very end of the sram memory section */
/* this is the "bottom" of the stack */
_estack = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
    /* put the .vectortable section on flash first, followed by the .text section (firmware code), followed by the .rodata section */
    .vectortable : { KEEP(*(.vectortable)) } > flash
    .text        : { *(.text*) }             > flash
    .rodata      : { *(.rodata*) }           > flash

    /* now place the .data section in sram */
    /* the dot ('.') is the location counter */
    /* it represents either an absolute address if used in the SECTIONS statement, or a byte offset if used in a section description */
    .data : {
        _data_start = .;
        *(.first_data)
        *(.data SORT(.data.*))
        _data_end = .;
    } > sram AT > flash

    _data_LMA = LOADADDR(.data);

    /* finally the .bss section */
    .bss : {
        _bss_start = .;
        *(.bss SORT(.bss.*) COMMON)
        _bss_end = .;
    } > sram
}

. = ALIGN(8);