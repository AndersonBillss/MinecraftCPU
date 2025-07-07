#include "binUtils.hpp"
#include <stdexcept>

uint8_t binUtils::toBigEndian8(int n)
{
    return static_cast<uint8_t>(n);
}

uint16_t binUtils::toBigEndian16(int n)
{
    uint16_t b = static_cast<uint16_t>(n);
    return (b >> 8) | (b << 8);
}

uint32_t binUtils::toBigEndian32(int n)
{
    uint32_t b = static_cast<uint32_t>(n);
    return ((b >> 24) & 0x000000FF) |
           ((b >> 8) & 0x0000FF00) |
           ((b << 8) & 0x00FF0000) |
           ((b << 24) & 0xFF000000);
}

uint64_t binUtils::toBigEndian64(int n)
{
    uint64_t b = static_cast<uint64_t>(n);

    return ((b >> 56) & 0x00000000000000FFULL) |
           ((b >> 40) & 0x000000000000FF00ULL) |
           ((b >> 24) & 0x0000000000FF0000ULL) |
           ((b >> 8) & 0x00000000FF000000ULL) |
           ((b << 8) & 0x000000FF00000000ULL) |
           ((b << 24) & 0x0000FF0000000000ULL) |
           ((b << 40) & 0x00FF000000000000ULL) |
           ((b << 56) & 0xFF00000000000000ULL);
}