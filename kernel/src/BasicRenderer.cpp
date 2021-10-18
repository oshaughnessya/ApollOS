//
// Created by Jake on 10/10/21.
//

#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font) {
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    Color = 0xffffffff;
    BackgroundColor = 0;
    CursorPosition = {OVER_SCAN, OVER_SCAN};
}

void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff) {
    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = Color;
            }
        }
        fontPtr++;
    }
}

void BasicRenderer::PutChar(char chr) {
    if (chr == '\n') {
        NewLine();
    } else {
        PutChar(chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X += 8;
        if (CursorPosition.X + 8 > TargetFramebuffer->Width - OVER_SCAN) {
            NewLine();
        }
    }
}

void BasicRenderer::Print(const char* str) {
    char* chr = (char*)str;
    while (*chr != 0) {
        PutChar(*chr);
        chr++;
    }
}

void BasicRenderer::Clear() {
    uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
    uint64_t bytesPerScanLine = TargetFramebuffer->PixelsPerScanLine * BPP;
    uint64_t fbHeight = TargetFramebuffer->Height;

    for (int verticalScanLine = 0; verticalScanLine < fbHeight; verticalScanLine++) {
        uint64_t pixPtrBase = fbBase + (bytesPerScanLine * verticalScanLine);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanLine); pixPtr++) {
            *pixPtr = BackgroundColor;
        }
    }
    CursorPosition = {OVER_SCAN, OVER_SCAN};
}

void BasicRenderer::ClearChar() {
    CursorPosition.X -= 8;
    if (CursorPosition.X < OVER_SCAN) {
        CursorPosition.X = OVER_SCAN + (8 * ((TargetFramebuffer->Width - 2 * OVER_SCAN) / 8 - 1));
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < OVER_SCAN) {
            CursorPosition.Y = OVER_SCAN;
        }
    }

    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    for (unsigned long y = CursorPosition.Y; y < CursorPosition.Y + 16; y++) {
        for (unsigned long x = CursorPosition.X; x < CursorPosition.X + 8; x++) {
            *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanLine)) = BackgroundColor;
        }
    }
}

void BasicRenderer::NewLine() {
    CursorPosition = {OVER_SCAN, CursorPosition.Y + 16};
    if (CursorPosition.Y + 16 + OVER_SCAN > TargetFramebuffer->Height) {
        Clear();
    }
}