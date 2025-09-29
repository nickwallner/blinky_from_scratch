#include "include/main.h"

/* reset handler, this is the very first function that is executed when the
mcu is is reset */
__attribute__((naked, noreturn)) void Reset_Handler(void)
{
    /* copy .data section to RAM and zero-initialize .bss section */
    extern long _data_start, _data_end, _bss_start, _bss_end, _data_LMA;
    for (long *dest = &_data_start, *src = &_data_LMA; dest < &_data_end;) *dest++ = *src++;
    for (long *dest = &_bss_start; dest < &_bss_end; dest++) *dest = 0;

    main();

    while(1) {} // infinite loop if main returns
}

extern void _estack(void); // initial stack pointer

/* vector table, 16 cortex vectors + 91 ST vectors */
__attribute__((section(".vectortable"))) void (* const vector_table[16 + 91])(void) = {
    _estack, Reset_Handler, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SYSTICK_Handler
};