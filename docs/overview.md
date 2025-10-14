## Introduction

Welcome to the blinky_from_scratch wiki! This wiki is designed to be a comprehensive guide
that walks you through the entire process of writing code from scratch to blink an LED with a
STM32 microcontroller. We won't be using any sort of HAL or IDE for this project. You just need your 
favorite text editor, a cross-compiler toolchain, and the reference manual for our MCU! Read on below
to learn how to get started.

### What does "from scratch" mean, exactly?

Many embedded software projects use some sort of HAL (hardware abstraction layer). A
HAL is designed to abstract away all the complicated memory organization, register reads and writes,
clock initializations, and much more from the user. This means that you, the programmer, can worry more about
making the MCU do what you want, and worry less about having to deal with configuring individual
peripherals. For many projects, this is ideal, because less time is spent reading the data sheet/reference
manual and more time is spent working on the actual project.

Personally, I believe that it is beneficial to know what goes on "behind the covers" so that
you can have a better overall understanding of how something works. Unfortunately, a HAL
covers so much that it makes learning the behind the scenes stuff very difficult. That's where
writing code from scratch comes in!

When I say that we will write all of our code "from scratch", that means we will be starting
with a completely blank slate. No IDEs, no HALs, no libraries (except for the C standard library).
Just you and your text editor (and some tools). Instead of using a preexisting HAL, we will write a minimal one ourself!
This way, we will gain an excellent understanding of everything thats happening behind the scenes,
so that once you start working on larger projects with a HAL or IDE (or both), you have a deeper
understanding of what the HAL is actually doing for you, which can make troubleshooting and debugging
a lot less painful.

### What do I need to get started?

First things first, you need to get an MCU! See [this](#choosing-a-development-board) section.

You'll also need to install a few tools if you haven't already. Here is a list of what you'll need:

- Your favorite text editor (I like [Visual Studio Code](https://code.visualstudio.com/download))
- A cross-compiler toolchain (see [this](#the-arm-none-eabi-toolchain) section)
- [GNU Make](https://www.gnu.org/software/make/) (optional, but highly recommended)
- [Okular](https://okular.kde.org/download/) pdf viewer (also optional, but highly recommended)

## Choosing a Development Board

Before you can start programming an MCU, you need to get your hands on one! Many MCU vendors sell **development boards**,
which provide a nice neat package that makes using the MCU easy. ST's calls their development boards
NUCLEO boards. We will be using a NUCLEO board throughout this guide.

If you look up ST's NUCLEO lineup, you'll see that there are quite a few boards to choose from. Each
board has a different MCU, but is otherwise essentially the same board. The board I'll be using is the
NUCLEO-F446RE board. You can find it [here](https://www.amazon.com/STM32-Nucleo-Development-STM32F446RE-NUCLEO-F446RE/dp/B01I8XLEM8/ref=sr_1_1?sr=8-1)
on amazon for about $30 (amazon links have a tendency to die; please let me know if this link dies so that I can fix it).
This board has a STM32F446RE mcu. If that specific board is not available, don't worry! Just look for a board with a
STM32F4 series MCU. All the F4 series MCUs are similar enough that you should still be able to follow this guide
without issue. You'll just have to double check your datasheets! More on that later.

These NUCLEO boards give you some built-in features, such as an led and pushbutton integrated in (we will use both).
The other very nice feature of these boards is that they include a **ST-Link** debugger/programmer, which makes flashing
our code extremely easy.

> Note: from here on out, when I refer to a "mcu", I am referring to the STM32F446RE

## The arm-none-eabi Toolchain

Our MCU has an ARM Cortex-M4 CPU. This means it can only run code for ARM architecture. In order to compile
code that can run on our mcu, we need a **cross-compiler**. A cross-compiler is a compiler that runs
on one architecture (x86-64, for instance), but compiles for another architecture (in our case, it will
compile for ARM architecture).

This guide will use the ARM GNU Toolchain, specifically the *arm-none-eabi* toolchain. You can
download the toolchain [here](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).

Some notes on downloading the toolchain:
- Make sure you download proper toolchain version for your host architecture. In my case, using Windows 11
on x86-64 architecture, I downloaded the **windows (mingw-w64-x86_64) hosted cross toolchain**
- Don't let the different names of the toolchain confuse you. You need to download *arm-none-eabi*. This
means the toolchain targets ARM architecture, has no vendor (none), and compiles using the ARM embedded
application binary interface (eabi)
- If you are on Windows, once you finish installing the toolchain, make sure you choose to add the toolchain
binaries to your PATH environment variable so that they can be accessed from the command prompt

Once you finish installing the toolchain, verify it was installed correctly:

```sh
$ arm-none-eabi-gcc --version

arm-none-eabi-gcc (Arm GNU Toolchain 14.3.Rel1 (Build arm-14.174)) 14.3.1 20250623
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## Other Tools

As I mentioned above, use your favorite text editor for this project. I advise against using
a full blown IDE (definitely don't use an embedded IDE like STM32CubeIDE or ARM Keil, that defeats
the purpose of writing everything from scratch!).

I recommend you install GNU Make if you haven't already so that you can automate the build process.
You can check if you have make installed by running:

```sh
$ make --version

GNU Make 3.81
Copyright (C) 2006  Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.
```

I also recommend you install Okular, or a similar pdf viewer. When we start programming, we will be
constantly using the **user reference manual** for our mcu. Reference manuals tend to be *very* long,
usually over 1,000 pages. Having a pdf viewer like Okular makes viewing these pdfs easy, since it allows you
to view by sections, rather than having to scroll through 500 pages to get to the section you want.

## Finding Datasheets and Reference Manuals

I've mentioned that we will be constantly relying on the reference manual for our mcu. Its like the
bible, but for embedded developers! Its important to understand that **datasheets** and **reference manuals**
are different:

Datasheet:
- Provides *specific* details about a mcu (flash size, memory size, cpu speed, etc)
- Lists all the features of the mcu
- Lists available packages for that mcu

Reference Manual:
- Typically applies to a whole series of mcus, not just individual ones
- Goes into detail about *literally everything* the mcu can do, and explains how to do it

We will mainly rely on the reference manual while programming. Finding datasheets and reference manuals
is pretty easy by just looking up the processor model in your favorite browser. Below are the
documents that you'll need, assuming you bought a NUCLEO-F446RE board like me:

- [Datasheet](https://www.st.com/resource/en/datasheet/stm32f446re.pdf)
- [Reference Manual](https://www.st.com/resource/en/reference_manual/rm0390-stm32f446xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [ARMv7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest/)
- [NUCLEO-F446RE User Manual](https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf)

If you purchased a board with a different mcu, you'll need to do some searching yourself to find
the correct datasheet and reference manual. However, the user manual for the NUCLEO board itself will likely be the same;
the one I linked is for all NUCLEO-F boards as well as some NUCLEO-L boards.

> Note: from here on out, when I refer to "the datasheet" or "the reference manual", I am referring to the
> datasheet/reference manual for the STM32F446RE