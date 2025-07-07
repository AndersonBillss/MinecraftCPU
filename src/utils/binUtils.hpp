#pragma once
#include <cstdint>

namespace binUtils {
    uint8_t toBigEndian8(int n);
    uint16_t toBigEndian16(int n);
    uint32_t toBigEndian32(int n);
    uint64_t toBigEndian64(int n);
}