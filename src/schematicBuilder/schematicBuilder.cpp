#include <iostream>
#include "exportSchematic.hpp"
#include "schematicBuilder.hpp"
#include "schematic.hpp"

Schematic createSchem(const std::string binary)
{
    const int memoryDimensions[] = {16, 24, 16};
    const int memorySpacing[] = {4, 2, 5};

    Schematic schem = {
        width : memoryDimensions[0] * memorySpacing[0],
        height : memoryDimensions[1] * memorySpacing[1],
        length : memoryDimensions[2] * memorySpacing[2],
        originX : 0,
        originY : 0,
        originZ : 0,
        blocks : {}
    };
     schem.originY = -schem.height;
     schem.originZ = -schem.width;
 
    for (int x = 0; x < schem.width; x++)
    {
        for (int y = 0; y < schem.height; y++)
        {
            for (int z = 0; z < schem.length; z++)
            {
                if (x % memorySpacing[0] == 0 && y % memorySpacing[1] == 0 && z % memorySpacing[2] == 0)
                {
                    schem.blocks.push_back(material::glass);
                }
                else
                {
                    schem.blocks.push_back(material::air);
                }
            }
        }
    }
    return schem;
}

void schematicBuilder::build(const std::string &binary)
{
    Schematic schem = createSchem(binary);
    std::string fileName = "example";
    std::string outputPath = "C:/Users/ander/AppData/Roaming/.minecraft/config/worldedit/schematics/" + fileName + ".schem";
    // std::string outputPath = "C:/Users/ander/Downloads/" + fileName + ".schem";

    ExportSchematic(schem, outputPath);
}