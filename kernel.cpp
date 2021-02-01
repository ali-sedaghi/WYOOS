#include "types.h"

// Declaring printf function beacaue we don't access to built-in libs
void printf(char* str) {
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for (int i = 0; str[i] != '\0'; ++i) {
        // Make sure not to write on color spaces
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
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
    while(1);
}