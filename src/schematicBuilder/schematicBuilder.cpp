#include <iostream>
#include "exportSchematic.hpp"
#include "schematicBuilder.hpp"
#include "schematic.hpp"
#include "../utils/stringUtils.hpp"

material getMaterial(int x, int y, int z, const std::vector<int> &dimensions, const std::string &bin)
{
    // Z and Y go from back to front
    z = dimensions[2] - z - 1;
    y = dimensions[1] - y - 1;
    
    size_t yIndex = y;
    size_t xIndex = x * dimensions[1];
    size_t zIndex = z * dimensions[0] * dimensions[1];
    size_t index = yIndex + xIndex + zIndex;

    if(index > bin.size()) return material::glass;
    return bin[index] == '1' ? material::redstoneBlock : material::glass;
}

Schematic createSchem(const std::string &binary)
{
    std::string trimmed = stringUtils::join(stringUtils::split(binary), "");

    const std::vector<int> memoryDimensions = {16, 24, 16};
    const std::vector<int> memorySpacing = {4, 2, 5};

    Schematic schem = {
        width : memoryDimensions[0] * memorySpacing[0],
        height : memoryDimensions[1] * memorySpacing[1],
        length : memoryDimensions[2] * memorySpacing[2],
        originX : 0,
        originY : 0,
        originZ : 0,
        blocks : {}
    };
    schem.originY = -schem.height + 1;
    schem.originZ = -schem.length + memorySpacing[2];

    for (int y = 0; y < schem.height; y++)
    {
        for (int z = 0; z < schem.length; z++)
        {
            for (int x = 0; x < schem.width; x++)
            {
                if ((x % memorySpacing[0] == 0) && (y % memorySpacing[1] == 0) && (z % memorySpacing[2] == 0))
                {
                    schem.blocks.push_back(getMaterial(
                        x / memorySpacing[0],
                        y / memorySpacing[1],
                        z / memorySpacing[2],
                        memoryDimensions,
                        trimmed));
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

void schematicBuilder::writeToFile(const std::string &outputPath, const std::string &binary)
{
    Schematic schem = createSchem(binary);
    ExportSchematic(schem, outputPath);
}