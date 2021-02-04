# Magic number that tells bootloader this is kernel
.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM


# For assigning stack pointer to kernel.cpp
.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader:
    mov $kernel_stack, %esp

    call callConstructors

    push %eax
    push %ebx
    call kernelMain

# Make sure kernel.cpp is in infinite loop
_stop:
    cli
    hlt
    jmp _stop

.section .bss
.space 2*1024*1024; # Free 2MB for stack to be pushed
kernel_stack:
