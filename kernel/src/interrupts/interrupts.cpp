//
// Created by Jake on 10/15/21.
//

#include "interrupts.h"

// TODO: add debug info once more string utilities are made
// TODO: try to recover
[[noreturn]] __attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame) {
    Panic("Page fault detected \n");

    while (true) {
        asm ("hlt");
    }
}

[[noreturn]] __attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame) {
    Panic("Double fault detected \n");

    while (true) {
        asm ("hlt");
    }
}

// TODO: try to recover
[[noreturn]] __attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame) {
    Panic("General Protection fault detected \n");

    while (true) {
        asm ("hlt");
    }
}

__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame) {
    uint8_t scancode = inb(0x60);
    HandleKeyboard(scancode);
    PIC_EndMater();
}

__attribute__((interrupt)) void PITInt_Handler(interrupt_frame* frame) {
    PIT::Tick();
    PIC_EndMater();
}

__attribute__((no_caller_saved_registers)) void PIC_EndMater() {
    outb(PIC1_COMMAND, PIC_EOI);
}

__attribute__((no_caller_saved_registers)) void PIC_EndSlave() {
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC() {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
    io_wait();
}