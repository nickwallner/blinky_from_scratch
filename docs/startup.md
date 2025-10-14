## Introduction

Before we start writing code for our embedded project (such as blinking an LED or controlling a motor),
we need to initialize our system so that it is in the proper state to start executing our project code.
Fortunately for us, the startup sequence follows a fairly simple pattern:

1. Copy program data from flash memory to SRAM and zero-initialize bss section
2. Call any peripheral initialization functions we may have
3. Call main()

Let's go ahead and look at how we write code to start up our microcontroller!

## The Vector Table

The very first thing we need to understand is our vector table. A vector table is simply an array of 32-bit
addresses that point to different things. The vast majority of those addresses point to interrupt handlers, which
are functions designed to handle different kinds of interrupts that can occur during code execution.

The ARM Cortex-M design specifies 16 standard vectors which are placed in the vector table
first. Then, the ST peripheral integration specifies additional vectors, placed after the 16 cortex vectors.
Reading our reference manual, we find that ST integrates 91 additional vectors into our MCU.
These vectors are placed in the vector table after the 16 cortex vectors. Below is a list of the 16 cortex vectors.
Note that these vectors are in a specific order, and that order *cannot be changed!* The same applies to the ST
peripheral vectors.

1. _estack (initial stack pointer)
2. Reset_Handler (startup code)
3. NMI_Handler (non-maskable interrupt)
4. HardFault_Handler (all class of faults)
5. MemManage_Handler (memory management)
6. BusFault_Handler (pre-fetch fault, memory access fault)
7. UsageFault_Handler (undefined instruction or illegal state)
8. 0
9. 0
10. 0
11. 0
12. SVC_Handler (system service call via SWI instruction)
13. DebugMon_Handler (debug monitor)
14. 0
15. PendSV_Handler (pendable request for system service)
16. SysTick_Handler (system tick timer)

I won't include a list of the 91 ST vectors because that would take up a lot of space (and you can go see
each vector for yourself in the reference manual on page 236).

## Writing our Vector Table

Let's look at how we would code our vector table. Often, the vendor for a MCU will provide some generic startup code
(usually written in assembly) that takes care of this for us. However, assembly is difficult to understand,
and we can achieve the same thing in C. Consider the following declaration:

```c
void (* const vector_table[16 + 91])(void);
```

This C statement can be translated to: "define an array of 16 + 91 pointers to functions that
take no arguments (void) and return nothing (void)". This is our vector table. Unfortunately, this
vector table currently has one fatal flaw: we don't know where it will be placed in memory. We know
that the vector table MUST be the very first thing in our flash memory. So how can we ensure our array
is placed into flash memory first? We can use a GNU-specific keyword called an attribute:

```c
__attribute__((section(".vectortable"))) void (* const vector_table[16 + 91])(void);
```

The `__attribute__` keyword is a feature specific to the GNU Compiler Collection (gcc), but has compatibility 
with many other compilers. It can be used to add specific characteristics to a variable or function. In this case, we are telling 
the compiler to place our `vector_table` array in a section called `.vectortable`. This way, when we write 
our linker script, we can access the `.vectortable` section and tell the linker to place everything in that section in flash memory first, before 
anything else. 

> The ((double parenthesis)) around the argument(s) passed to `__attribute__` may seem redundant. However,
> there is a good reason for why they are there. __*So don't omit them!*__ Refer to the gcc documentation for more
> information about `__attribute__`.

So far, we've defined an array of function pointers as our vector table, and we have placed our vector
table in a special section for the linker to use later. But right now, our vector table is empty, so
we need to add some vectors to it! 

We know that the first entry in our vector table needs to be the initial stack pointer. But how can we get the
value of the initial stack pointer? The simple answer is through the linker script. Refer to [this](#writing-our-linker-script) section. 
For now, lets look at what we need to do in our C file:

```c
extern void _estack(void);
```

We declare an external symbol `_estack`, which will be defined by the linker script. This will be our initial
stack pointer. We can simply add this function to our vector table:

```c
extern void _estack(void);

__attribute__((section(".vectortable"))) void (* const vector_table[16 + 91])(void) = {
    _estack
};
```

And viola! We've added our first entry to our vector table. Next up, we need to write our reset handler and add it too.
Let's see what that looks like:

```c
extern void _estack(void);

__attribute__((naked, noreturn)) void Reset_Handler(void) 
{
    while(1) {} // infinite loop
}

__attribute__((section(".vectortable"))) void (* const vector_table[16 + 91])(void) = {
    _estack, Reset_Handler
};
```

Now we've defined a function `Reset_Handler()`, which for the time being will just loop infinitely. Notice that
we used the `__attribute__` keyword again. This time, we declared our function as `naked`, which tells the compiler
not to generate a prologue or epilogue for our function (prologue and epilogue refer to the instructions that the 
compiler inserts automatically before and after a function call, such as saving/restoring registers or managing
the stack pointer) and `noreturn`, which lets the compiler know that this function will *never* return, suppressing
possible warnings/errors.

Now we have a more complete vector table! This is as bare-bones as a piece of firmware can get. If we wanted to, we
could actually compile this code along with a linker script, flash it to our MCU, and run it! Obviously, though, it wouldn't
do anything (that we can see anyway). Before we flesh out our `Reset_Handler()` more, lets look at how we write
a linker script. That way, we can understand what more needs to be done with our `Reset_Handler()`.

## Writing our Linker Script

Linking is a part of the compilation process that is almost always completely abstracted away from the
programmer. As you likely already know, the last part of the compilation process is linking, where the
*linker* goes through each object file that the compiler produces and combines sections and resolves references between files (among other things).
The linker produces a **single** executable file, which can have different extensions based on the architecture it was
compiled for (`.exe`, `.app`, etc). For our purposes of embedded programming on ARM architecture, the linker will output a `.elf` file.

Every time the linker runs, it reads a *linker script*, which tells the linker what the output file should look like.
Some of the things we do in a linker script are:

- define a code entry point for our processor
- define memory regions (such as flash memory and RAM)
- place sections of code at specific memory addresses
- define symbols that are declared in our startup code (such as the stack pointer)

Let's go ahead a write a minimal linker script to use with our startup code!

The very first line of our linker script defines the code entry point using the `ENTRY` command. This tells the processor where
it should jump to first when it starts executing our code. However, this command technically is not actually necessary for us, because our cortex core will
*always* jump to the reset vector in the vector table to begin code execution. However, its good practice to include the `ENTRY`
command in our linker script because it can help us during debugging.

```
ENTRY(Reset_Handler);
```

Our entry point is just the `Reset_Handler()` that we wrote in the above section! So, when our processor is reset,
the *very first* function it will execute is `Reset_Handler()`.

Next, we need to tell the linker what memory regions we will be using. Right now, we have two sections
that we'll use: flash memory and SRAM. Let's define those two sections in our linker script:

```
MEMORY {
    flash (rx)  : ORIGIN = 0x08000000, LENGTH = 128M
    sram  (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
}
```

These next few sentences are *very important*, so read carefully! The value of `ORIGIN` for your memory regions
may be different depending on your microcontroller. You need to read the **reference manual** (specifically, read the
**memory map**) for *your specific microcontroller* to find the correct base memory addresses for your flash and sram. 
If you are using an STM32F446RE microcontroller, you can just copy this code verbatim. However, if you are using a 
different microcontroller, the origins for your memory regions may be different! So double check your datasheet
before you copy my code.

Let's break down the definition for our `flash` memory region:
- `flash` is simply the name of the memory region
- `(rx)` means that we can **r**ead and e**x**ecute from this memory region. We can't write to it!
- `ORIGIN` is the starting memory address for our memory region, obtained from our microcontroller's **memory map**
- `LENGTH` is the size of the memory region. In this case, our flash memory is 128MiB, which we learned by reading the **datasheet**

The definition for our `sram` region is very similar. Note that sram has `(rwx)` instead of just `(rx)`
like flash. This means that we can read, write, and execute with sram!

Next up, let's define our stack pointer! In the section above, [Writing our Vector Table](#writing-our-vector-table), 
we created a symbol `_estack` in our startup code and placed it
in our vector table. However, we didn't define it, because we don't immediately know what our stack pointer value
will be. Fortunately, the linker script does, so let's define it now:

```
_estack = ORIGIN(sram) + LENGTH(sram);
```
This defines the symbol `_estack` as the *last* memory address in our sram. This is because the stack grows
'downwards'. In other words, the stack starts at the max memory address for our sram, and moves to lower (smaller)
memory addresses as the stack increases in size. To make things even more confusing, we typically refer to this
max memory address as the "bottom" of the stack, even though its technically at the top of our memory region!

If we look at the memory map for the STM32F446RE, we see that the upper address for sram is `0x2001 BFFF`. That's
the value that our stack pointer will have initially. Now, let's suppose we add a word of data to the stack (remember
a word is 32 bits!). Our stack pointer would be *decremented* by 4 bytes (32 bits), and its new value would be
`0x2001 BFFC`.

Now its time to write the main body of our linker script. Recall that the purpose of the linker is to take multiple
inputs and make one ouput. The compiler compiles our `.c` files *individually*, and outputs `.o` files (object
 files) in the ELF binary format. These `.o` files contain different **sections** that hold different data. For instance,
the `.text` section contains the actual program code. The `.data` section contains program data (variables). Other sections include
the `.bss` section, which holds uninitialized data, and sections for the debugger. When the linker runs, it takes each of the `.o` files and combines them into
one single `.elf` file. We need to tell our linker how it should place each of our program sections into
the final firmware file.

To achieve this, we use the `SECTIONS` command in the linker script. The `SECTIONS` command tells our linker how it
should map input sections to output sections, and also tells it where those output sections should be placed in
memory. The full SECTIONS part of our linker script looks like this:

```
SECTIONS {
    .vectortable : { KEEP(*(.vectortable)) } > flash
    .text        : { *(.text*) }             > flash
    .rodata      : { *(.rodata*) }           > flash

    .data : {
        _data_start = .;
        *(.first_data)
        *(.data SORT(.data.*))
        _data_end = .;
    } > sram AT > flash

    _data_LMA = LOADADDR(.data);

    .bss : {
        _bss_start = .;
        *(.bss SORT(.bss.*) COMMON)
        _bss_end = .;
    } > sram
}
```

Let's break this down into smaller parts to understand it better. We start with the `SECTIONS` command, using
curly braces to surround our output sections.

We start by placing 3 sections in flash memory:

```
.vectortable : { KEEP(*(.vectortable)) } > flash
.text        : { *(.text*) }             > flash
.rodata      : { *(.rodata*) }           > flash
```

Remember when we wrote our vector table in C in the section above and used the `__attribute__` feature to add our
vector table to its own section called `.vectortable`? The `.vectortable` section you see here is the same as
the one we defined in our startup code. We know that the microcontroller expects the vector table to be the
very first thing in flash memory, so we are doing just that: placing our `.vectortable` section in flash
memory first, before anything else.

Let's take a closer look at one of these output section definitions to try and understand the syntax better:

```
.vectortable : { KEEP(*(.vectortable)) } > flash
```

We start with the *name* of the output section. In this case, its `.vectortable`. Then, a colon, followed
by curly brackets, and then followed by commands for that specific output section. 

In this output section, we use the `KEEP` command, which tells the linker to place this section in memory 
*regardless* of if it is completely or partially empty. This is important for our vector table because the vector 
table MUST be present in flash memory, regardless of if it has some empty parts. We don't want any part of it to be
skipped or omitted.

Next, we use the wildcard operator `*`. This allows us to match any file name. In this scenario, we are using
it to tell the linker to include ANY files that end with ".vectortable" in this output section.

Finally, we use the `>` ouput operator and specify a memory region to place this output section in. Obviously,
we want our vector table in flash memory, so we specify the flash memory region that we defined earlier.

The next part of the SECTIONS portion of our linker script is similar to what we just went over:

```
.data : {
    _data_start = .;
    *(.first_data)
    *(.data SORT(.data.*))
    _data_end = .;
} > sram AT > flash

_data_LMA = LOADADDR(.data);

.bss : {
    _bss_start = .;
    *(.bss SORT(.bss.*) COMMON)
    _bss_end = .;
} > sram
```

This code is still simply defining output sections, in this case it defines output sections `.data` and `.bss`.
In addition to the output sections, it is also defining some symbols (`_data_start, _data_end, _bss_start, _bss_end`),
which we will use in our startup code.

I'm not going to go into any more detail about the linker script; that could take up a lot more space.
I highly recommend taking some time to read over [this](https://users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html)
resource. It is an excellent explanation of how a linker script is written.

## Writing our Startup Code

Right now, we have a framework for our startup code and a basic linker script. All that's left to do is to finish our startup
code!

Our `Reset_Handler()` needs to do a few things:
1. Copy the `.data` section from flash to RAM
2. Zero-initialize the `.bss` section
3. Call `main()`

Let's write some code for each of those steps, starting with step #1:

```c
extern long _data_start, _data_end, _bss_start, _bss_end, _data_LMA;
```

We start by defining some external symbols. Notice that these are the same symbols we defined in our
linker script! These symbols are simply memory addresses which we will use to loop through the different sections:

```c
for (long *dest = &_data_start, *src = &_data_LMA; dest < &_data_end;) *dest++ = *src++;
```

What this loop does is not necessarily intuitive. Before you continue, you *need* to know what the Load Memory Address
(LMA) and Virtual Memory Address (VMA) are from our linker script. Look up the terms if you don't know them;
I won't explain it here.

This loop has two loop variables: `dest`, which is the destination for our data to be copied to and is initially
assigned the value of the `.data` section's VMA. We also have the `src` variable, which is the source that
we get our data from and is initially assigned the value of the `.data` section's LMA. Our loop condition,
`dest < &_data_end`, makes sure we don't iterate past our `.data` section. Finally, in the loop body, we
assign the value of `src` to `dest` after dereferencing both variables. This copies the `.data` section
from flash memory to SRAM.

We use a very similar `for` loop to zero-initialize our `.bss` section:

```c
for (long *dest = &_bss_start; dest < &_bss_end; dest++) *dest = 0;
```

All that's left to do is to put these loops in our `Reset_Handler()` and add a `main()` function. Let's do that:

```c
int main()
{
    return 0; // do nothing
}

__attribute__((naked, noreturn)) void Reset_Handler(void) 
{
    /* copy .data section to RAM and zero-initialize .bss section */
    extern long _data_start, _data_end, _bss_start, _bss_end, _data_LMA;
    for (long *dest = &_data_start, *src = &_data_LMA; dest < &_data_end;) *dest++ = *src++;
    for (long *dest = &_bss_start; dest < &_bss_end; dest++) *dest = 0;

    main();

    while(1) {} // infinite loop if main returns
}

extern void _estack(void);

__attribute__((section(".vectors"))) void (* const vector_table[16 + 91])(void) = {
    _estack, Reset_Handler
};
```

And that's it! This code, along with our linker script, is enough to make a bare-bones piece of firmware
that does nothing. When we start to write project code, we can simply reuse this startup code and linker
script with minimal changes. As a final step, let's briefly look over how we would compile and flash this code.

## Compilation and Flashing

You should already have the `arm-none-eabi` toolchain installed. First, we compile our code to get `.o` file(s):

```sh
$ arm-none-eabi-gcc -mcpu=cortex-m4 main.c -c
```

The `-c` option tells `arm-none-eabi-gcc` to compile our code but not link it.

Then we can link our `.o` file(s) using our linker script to make an `.elf` executable:

```sh
$ arm-none-eabi-gcc -T link.ld -nostdlib main.o -o firmware.elf
```

The `-T` option specifies the linker script to use, and the `-nostdlib` option tells the linker not to include
any standard startup files or libraries.

Now we have a complete firmware file! Let's examine the sections of our file:

```sh
$ arm-none-eabi-objdump -h firmware.elf

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .vectortable  000001ac  08000000  08000000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .text         00000050  080001ac  080001ac  000011ac  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
```

We see that the very first section in our file is the `.vectortable` section, which is exactly what we want!
It will be loaded to flash memory first, starting at address `0x08000000`, which is the base address for flash memory. 
After our vector table comes the `.text` section, which is our program code. Its loaded in to flash memory starting
at address `0x0800001AC`, which is immediately after our vector table. Normally, there would also be a `.data` section
and maybe a `.bss` section. But right now, our code does nothing and has no variables! So the `.data` section and `.bss`
section are both empty, and thus not included in the final executable.

The last thing we need to do before flashing our firmware to our MCU is to make a `.bin` file from our `.elf` file using
our compiler toolchain:

```sh
$ arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
```

Now, flashing our code to the microcontroller should be fairly easy. If you have a ST NUCLEO development board,
it will have a ST-Link debugger/programming built in. If that is the case, flashing is very easy, since we can
use the built-in bootloader in the ST-Link debugger/programmer:
1. Plug your board in to your computer via USB
2. The board should appear as a file device (just like a usb drive would)
3. Just drag and drop your `.bin` file to the file device
4. The ST-Link LED will alternate red and green while the file is flashing
5. If the file flashed successfully, the LED should return to solid green, and you are done!

You can also flash via ST's command line utility `st-flash`. After plugging your board in via USB, run:

```sh
$ st-flash --reset write firmware.bin 0x80000000
```

If you have some other development board, you'll need to do some research to figure out
what sort of flashing options are available to you.

## Wrapping Up

Take a moment and pat yourself on the back. You've just made a piece of firmware *completely from scratch*!
Next up, we'll learn how to write some project code and then will blink an LED. Onward!