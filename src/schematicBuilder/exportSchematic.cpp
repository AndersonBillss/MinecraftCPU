#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <zlib.h>
#include "../utils/binUtils.hpp"
#include "exportSchematic.hpp"

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

void write_gzip(const std::string &filename, const std::vector<uint8_t> &data)
{
    gzFile file = gzopen(filename.c_str(), "wb");
    if (!file)
        throw std::runtime_error("Failed to open gzip file");
    gzwrite(file, data.data(), data.size());
    gzclose(file);
}

void ExportSchematic(Schematic schem, std::string outputPath)
{
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

    tagName = "WEOffsetX";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, schem.originX);

    tagName = "WEOffsetY";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, schem.originY);

    tagName = "WEOffsetZ";
    buffer.push_back(tag::tagInt);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt32(buffer, schem.originZ);

    buffer.push_back(tag::tagEnd);

    tagName = "Width";
    buffer.push_back(tag::tagShort);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt16(buffer, schem.width);

    tagName = "Height";
    buffer.push_back(tag::tagShort);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt16(buffer, schem.height);

    tagName = "Length";
    buffer.push_back(tag::tagShort);
    pushInt16(buffer, tagName.size());
    buffer.insert(buffer.end(), tagName.begin(), tagName.end());
    pushInt16(buffer, schem.length);

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
    pushInt32(buffer, schem.blocks.size());
    for (material block : schem.blocks)
    {
        buffer.push_back(block);
    }

    buffer.push_back(tag::tagEnd);

    write_gzip(outputPath, buffer);
}