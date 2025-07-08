#pragma once
#include <vector>

namespace schematic {

};
enum material
{
    air,
    glass,
    redstoneBlock
};

struct Schematic
{
    int width;
    int height;
    int length;
    int originX;
    int originY;
    int originZ;
    std::vector<material> blocks;
};

