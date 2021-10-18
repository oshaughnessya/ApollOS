//
// Created by Jake on 10/15/21.
//

#include "panic.h"

__attribute__((no_caller_saved_registers)) void Panic(const char* panicMessage) {
    GlobalRenderer->BackgroundColor = 0x00ff0000;
    GlobalRenderer->Clear();

    GlobalRenderer->CursorPosition = {OVER_SCAN, OVER_SCAN};
    GlobalRenderer->Color = 0;

    GlobalRenderer->Print("Kernel Panic \n\n");
    GlobalRenderer->Print(panicMessage);
}