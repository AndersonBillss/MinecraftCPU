#include "schematicBuilder.hpp"
enum material
{
    air = 0,
    glass = 20,
    redstoneBlock = 152
};
struct Schematic {
    int width;
    int height;
    int length;
    material blocks[];
};
void schematicBuilder::build(std::string binary)
{
}
