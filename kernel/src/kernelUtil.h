//
// Created by Jake on 10/15/21.
//

#pragma once
#include "FrameBuffer.h"
#include "simpleFonts.h"
#include "memory/efiMemory.h"
#include "pci/acpi.h"
#include "BasicRenderer.h"

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
