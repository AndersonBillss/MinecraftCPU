#pragma once
#include "schematic.hpp"
#include <string>

enum tag
{
    tagCompound = 0x0A,
    tagShort = 0x02,
    tagInt = 0x03,
    tagByteArray = 0x07,
    tagEnd = 0x00
};

void ExportSchematic(Schematic schem, std::string outputPath);