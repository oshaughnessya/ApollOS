//
// Created by Jake on 10/16/21.
//

#pragma once
#include <cstddef>

struct HeapSegHdr {
    size_t length;
    HeapSegHdr* next;
    HeapSegHdr* last;
    bool free;
    void CombineForward();
    void CombineBackward();
    HeapSegHdr* Split(size_t splitLength);
};

void InitializeHeap(void* heapAddress, size_t pageCount);

void* malloc(size_t size);
void free(void* address);

void ExpandHeap(size_t length);

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* p);
void operator delete(void* p, size_t size);