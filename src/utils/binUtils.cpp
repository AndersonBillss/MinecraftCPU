#include "binUtils.hpp"
#include <stdexcept>

uint8_t binUtils::toBigEndian8(unsigned int n)
{
    if (n < 0 || n > 255)
    {
        throw std::out_of_range("8 bit number must be between 0 and 255");
    }
    uint8_t bit = 0b00000001;

    return static_cast<uint8_t>(n);
}

uint16_t binUtils::toBigEndian16(unsigned int n)
{
    if (n < 0 || n > 65536)
    {
        throw std::out_of_range("16 bit number must be between 0 and 65536");
    }
    return (n >> 8) | (n << 8);
}

uint32_t binUtils::toBigEndian32(unsigned int n)
{
    if (n < 0 || n > 4294967296)
    {
        throw std::out_of_range("32 bit number must be between 0 and 4294967296");
    }
    return ((n >> 24) & 0x000000FF) |
           ((n >> 8) & 0x0000FF00) |
           ((n << 8) & 0x00FF0000) |
           ((n << 24) & 0xFF000000);
}

uint64_t binUtils::toBigEndian64(unsigned int n)
{
    if (n < 0 || n > 18446744073709551616)
    {
        throw std::out_of_range("64 bit number must be between 0 and 18446744073709551616");
    }

    return ((n >> 56) & 0x00000000000000FFULL) |
           ((n >> 40) & 0x000000000000FF00ULL) |
           ((n >> 24) & 0x0000000000FF0000ULL) |
           ((n >> 8) & 0x00000000FF000000ULL) |
           ((n << 8) & 0x000000FF00000000ULL) |
           ((n << 24) & 0x0000FF0000000000ULL) |
           ((n << 40) & 0x00FF000000000000ULL) |
           ((n << 56) & 0xFF00000000000000ULL);
}