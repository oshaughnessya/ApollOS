//
// Created by Jake on 10/15/21.
//

#pragma once
#include <cstdint>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();

