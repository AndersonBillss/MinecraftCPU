#pragma once
#include <cstdint>

namespace binUtils {
    uint8_t toBigEndian8(unsigned int n);
    uint16_t toBigEndian16(unsigned int n);
    uint32_t toBigEndian32(unsigned int n);
    uint64_t toBigEndian64(unsigned int n);
}