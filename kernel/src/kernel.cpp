//
// Created by Jake on 10/10/21.
//
#include "kernelUtil.h"

extern "C" [[noreturn]] void _start(BootInfo* bootInfo) {
    InitializeKernel(bootInfo);

    GlobalRenderer->Print("Kernel Initialized Successfully \n");

    while (true) {
        asm ("hlt");
    }
}