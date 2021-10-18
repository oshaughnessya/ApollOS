//
// Created by Jake on 10/11/21.
//
#include "efiMemory.h"

const char* EFI_MEMORY_TYPE_STRINGS[] {
    "EfiReservedMemoryType",        // 0
    "EfiLoaderCode",                // 1
    "EfiLoaderData",                // 2
    "EfiBootServicesCode",          // 3
    "EfiBootServicesData",          // 4
    "EfiRuntimeServicesCode",       // 5
    "EfiRuntimeServicesData",       // 6
    "EfiConventionalMemory",        // 7
    "EfiUnusableMemory",            // 8
    "EfiACPIReclaimMemory",         // 9
    "EfiACPIMemoryNVS",             // 10
    "EfiMemoryMappedIO",            // 11
    "EfiMemoryMappedIOPortSpace",   // 12
    "EfiPalCode",                   // 13
};
