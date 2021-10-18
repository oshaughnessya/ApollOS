//
// Created by Jake on 10/16/21.
//

#include "pit.h"

namespace PIT {
    uint64_t TimeSinceBoot = 0;

    uint16_t Divisor = 65535;

    void Sleep(uint64_t milliseconds) {
        uint64_t startTime = TimeSinceBoot;
        while (TimeSinceBoot < startTime + milliseconds) {
            asm("hlt");
        }
    }

    void SetDivisor(uint16_t divisor) {
        if (divisor < 100) {
            divisor = 100;
        }
        Divisor = divisor;
        outb(0x40, (uint8_t)(divisor & 0x00ff));
        io_wait();
        outb(0x40, (uint8_t)((divisor & 0xff00) >> 8));
    }

    uint64_t GetFrequency() {
        return BaseFrequency / Divisor;
    }

    void SetFrequency(uint64_t frequency) {
        SetDivisor(BaseFrequency / frequency);
    }

    __attribute__((no_caller_saved_registers)) void Tick() {
        TimeSinceBoot += 1000 / GetFrequency();
    }
}