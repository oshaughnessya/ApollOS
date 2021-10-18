//
// Created by Jake on 10/15/21.
//

#pragma once
#include "FrameBuffer.h"
#include "simpleFonts.h"
#include "BasicRenderer.h"
#include "cstr.h"
#include "memory/efiMemory.h"
#include "memory/memory.h"
#include "Bitmap.h"
#include "paging/paging.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageTableManager.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "pci/acpi.h"
#include "pci/pci.h"
#include "memory/heap.h"

struct BootInfo {
    Framebuffer* framebuffer;
    PSF1_FONT* psf1_Font;
    EFI_MEMORY_DESCRIPTOR* mMap;
    uint64_t mMapSize;
    uint64_t mMapDescSize;
    ACPI::RSDP2* rsdp;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

void InitializeKernel(BootInfo* bootInfo);
