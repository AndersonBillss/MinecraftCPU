#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <zlib.h>
#include "../utils/binUtils.hpp"

#include "schematicBuilder.hpp"

enum material
{
    air,
    glass,
    redstoneBlock
};
enum tag
{
    tagCompound = 0x0A,
    tagShort = 0x02,
    tagInt = 0x03,
    tagByteArray = 0x07,
    tagEnd = 0x00
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

void pushInt8(std::vector<uint8_t> &buffer, const int &n)
{
    buffer.push_back(n);
}
void pushInt16(std::vector<uint8_t> &buffer, const int &n)
{
    int b = binUtils::toBigEndian16(n);
    buffer.push_back(b);
    buffer.push_back(b >> 8);
}
void pushInt32(std::vector<uint8_t> &buffer, const int &n)
{
    int b = binUtils::toBigEndian32(n);
    buffer.push_back(b);
    buffer.push_back(b >> 8);
    buffer.push_back(b >> 16);
    buffer.push_back(b >> 24);
}

void write_gzip(const std::string& filename, const std::vector<uint8_t>& data) {
    gzFile file = gzopen(filename.c_str(), "wb");
    if (!file) throw std::runtime_error("Failed to open gzip file");
    gzwrite(file, data.data(), data.size());
    gzclose(file);
}

void schematicBuilder::build(std::string binary)
{
    Schematic exampleSchem = {
        width : 3,
        height : 3,
        length : 3,
        originX : 0,
        originY : 0,
        originZ : 0,
        blocks : {
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
    std::string fileName = "example";
    std::string outputPath = "C:/Users/ander/AppData/Roaming/.minecraft/config/worldedit/schematics/" + fileName + ".schem";
    std::cout << "HELLO SCHEMATIC" << std::endl;
    std::vector<uint8_t> buffer;

    std::string tagName = "Schematic";
    buffer.push_back(tag::tagCompound);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());

    tagName = "Version";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, 2);

    tagName = "DataVersion";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, 3465);
    
    tagName = "Metadata";
    buffer.push_back(tagCompound);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());

    tagName = "WEOriginX";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, exampleSchem.originX);

    tagName = "WEOriginY";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, exampleSchem.originY);

    tagName = "WEOriginZ";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, exampleSchem.originZ);

    buffer.push_back(tag::tagEnd);

    tagName = "Width";
    buffer.push_back(tag::tagShort);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt16(buffer, exampleSchem.width);

    tagName = "Height";
    buffer.push_back(tag::tagShort);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt16(buffer, exampleSchem.height);

    tagName = "Length";
    buffer.push_back(tag::tagShort);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt16(buffer, exampleSchem.length);

    tagName = "PalleteMax";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, 3);

    tagName = "Palette";
    buffer.push_back(tagCompound);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());

    tagName = "minecraft:air";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, material::air);

    tagName = "minecraft:glass";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, material::glass);

    tagName = "minecraft:redstone_block";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, material::redstoneBlock);

    buffer.push_back(tag::tagEnd);

    tagName = "BlockData";
    buffer.push_back(tag::tagByteArray);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, exampleSchem.blocks.size());
    for (material block : exampleSchem.blocks)
    {
        buffer.push_back(block);
    }

    buffer.push_back(tag::tagEnd);

    write_gzip(outputPath, buffer);
}
