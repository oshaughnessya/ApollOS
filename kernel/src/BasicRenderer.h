//
// Created by Jake on 10/10/21.
//
#pragma once
#include "FrameBuffer.h"
#include "simpleFonts.h"
#include "math.h"

#define BPP 4
#define OVER_SCAN 3

class BasicRenderer {
public:
    BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font);
    Point CursorPosition;
    Framebuffer* TargetFramebuffer;
    PSF1_FONT* PSF1_Font;
    unsigned int Color;
    unsigned int BackgroundColor;
    void PutChar(char chr);
    void Print(const char* str);
    void Clear();
    void ClearChar();

private:
    void PutChar(char chr, unsigned int xOff, unsigned int yOff);
    void NewLine();
};

extern BasicRenderer* GlobalRenderer;