//
// Created by Jake on 10/15/21.
//

#pragma once


#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0

#define PIC_EOI 0x20

#define PIC2_DATA 0xA1
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame;

[[noreturn]] __attribute__((interrupt)) __attribute__((no_caller_saved_registers)) void PageFault_Handler(interrupt_frame* frame);
[[noreturn]] __attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame);
[[noreturn]] __attribute__((interrupt)) __attribute__((no_caller_saved_registers)) void GPFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) __attribute__((no_caller_saved_registers)) void KeyboardInt_Handler(interrupt_frame* frame);
__attribute__((interrupt)) __attribute__((no_caller_saved_registers)) void PITInt_Handler(interrupt_frame* frame);

void RemapPIC();
void PIC_EndMater();
void PIC_EndSlave();