//
// Created by Jake on 10/16/21.
//

#pragma once
#include <cstdint>

namespace PIT {
    extern uint64_t TimeSinceBoot;
    const uint64_t BaseFrequency = 1193182;

    void Sleep(uint64_t milliseconds);

    void SetDivisor(uint16_t divisor);
    uint64_t GetFrequency();
    void SetFrequency(uint64_t frequency);
    void Tick();
}
