#include "types.h"
#include "gdt.h"
#include "interrupts.h"

// Declaring printf function beacaue we don't access to built-in libs
void printf(char* str) {
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    // For printing more than one time
    // Screen Size 80 * 25
    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        switch (str[i]) {

            // Newline
            case '\n':
                y++;
                x = 0;
                break;

            // Other chars
            default:
                // Make sure not to write on color spaces
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }

        // Check the cursor location
        if (x >= 80) {
            y++;
            x = 0;
        }
        if (y >= 25) {
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            
            x = 0;
            y = 0;
        }
    }        
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for (constructor* i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
}

// extern tell g++ not to change the function name on object file
extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*magicnumber*/) {
    printf("Hello World! Welcome to nikomadol OS.");
    printf("Hey");

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(&gdt);
    interrupts.Activate();
    
    while(1);
}