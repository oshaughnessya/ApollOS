//
// Created by Jake on 10/15/21.
//

#include "kernelUtil.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageTableManager.h"
#include "memory/memory.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "scheduling/pit/pit.h"
#include "pci/pci.h"
#include "gdt/gdt.h"
#include "memory/heap.h"
#include "IO.h"

void PrepareMemory(BootInfo* bootInfo) {
    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / PAGE_SIZE + 1;

    GlobalAllocator.LockPages(&_KernelStart, kernelPages);

    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, PAGE_SIZE);

    GlobalPageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize); t += PAGE_SIZE) {
        GlobalPageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + PAGE_SIZE;
    GlobalAllocator.LockPages((void*)fbBase, fbSize / PAGE_SIZE + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += PAGE_SIZE) {
        GlobalPageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));
}

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector) {
    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}

void PrepareInterrupts() {
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

    SetIDTGate((void*)PageFault_Handler, 0x0e, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFault_Handler, 0x08, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_Handler, 0x0d, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);
    PIT::SetDivisor(65535);
    SetIDTGate((void*)PITInt_Handler, 0x20, IDT_TA_InterruptGate, 0x08);

    asm("lidt %0" : : "m" (idtr));

    RemapPIC();
}

void PrepareACPI(BootInfo* bootInfo) {

    ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(bootInfo->rsdp->XSDTAddress);

    ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");

    PCI::EnumeratePCI(mcfg);
}

BasicRenderer renderer = BasicRenderer(nullptr, nullptr);
void InitializeKernel(BootInfo* bootInfo) {
    renderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);
    GlobalRenderer = &renderer;

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);

    PrepareMemory(bootInfo);

    InitializeHeap((void*)0x0000100000000000, 0x10);

    // set the entire screen black
    memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);

    PrepareInterrupts();

    PrepareACPI(bootInfo);

    outb(PIC1_DATA, 0b11111100);
    outb(PIC2_DATA, 0b11111111);

    asm("sti");

}