//
// Created by Jake on 10/15/21.
//

#include "keyboard.h"

bool isLeftShiftPressed = false;
bool isRightShiftPressed = false;

void HandleKeyboard(uint8_t scancode) {
    switch (scancode) {
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
//        case Enter:
//            GlobalRenderer->PutChar('\n');
//            return;
        case BackSpace:
            GlobalRenderer->ClearChar();
            return;
        case SpaceBar:
            GlobalRenderer->PutChar(' ');
            return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0) {
        GlobalRenderer->PutChar(ascii);
    }
}