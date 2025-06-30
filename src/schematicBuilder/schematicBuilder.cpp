#include <vector>
#include <iostream>
#include <cstdint>

#include "schematicBuilder.hpp"

enum material
{
    air = 0b00000000, //0
    glass = 0b00010100, //2
    redstoneBlock = 0b10011000 //152
};
struct Schematic {
    int width;
    int height;
    int length;
    std::vector<material> blocks;
};
void schematicBuilder::build(std::string binary)
{
    Schematic exampleSchem = {
        width: 3,
        height: 3,
        length: 3,
        blocks: {
            material::air,
            material::glass,
            material::redstoneBlock,
            material::air,
            material::glass,
            material::redstoneBlock,
            material::air,
            material::glass,
            material::redstoneBlock,
            
            material::redstoneBlock,
            material::glass,
            material::air,
            material::redstoneBlock,
            material::glass,
            material::air,
            material::redstoneBlock,
            material::glass,
            material::air,

            material::air,
            material::glass,
            material::redstoneBlock,
            material::air,
            material::glass,
            material::redstoneBlock,
            material::air,
            material::glass,
            material::redstoneBlock,   
        }
    };
    std::string fileName = "example.schem";
    std::string outputPath = "C:/Users/ander/AppData/Roaming/.minecraft/config/worldedit/schematics/" + fileName;
    std::cout << "HELLO SCHEMATIC" << std::endl;
    std::vector<uint8_t> buffer;
    buffer.push_back(0x0A);

}

