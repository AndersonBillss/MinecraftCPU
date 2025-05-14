import math
import mcschematic
import os

MEMORY_DIMENSIONS = [16,24,16]
MEMORY_SPACING = [4,2,5]

def createLine(schem, binLine, count):
    offsetX = count%MEMORY_DIMENSIONS[0]*MEMORY_SPACING[0]
    offsetZ = math.floor(count/MEMORY_DIMENSIONS[2])*MEMORY_SPACING[2]

    width = MEMORY_DIMENSIONS[2]*MEMORY_SPACING[2]-1
    height = MEMORY_DIMENSIONS[1]*MEMORY_SPACING[1]-1
    
    splitBinLine = binLine.split()
    binLine = ""
    for item in splitBinLine:
        binLine += item

    for (i, char) in enumerate(binLine):
        block = "minecraft:glass"
        if(char == "1"):
            block = "minecraft:redstone_block"

        schem.setBlock((offsetX, (-i*MEMORY_SPACING[1]-1), -offsetZ), block)


def createSchem(binList, schemOutput):
    schem = mcschematic.MCSchematic()
    n=MEMORY_DIMENSIONS[0] * MEMORY_DIMENSIONS[2]

    for i in range(n):
        if i < len(binList):
            line = binList[i]
            createLine(schem, line, i)
        else:
            createLine(schem, "00000000 00000000 00000000", i)

    schemPath = os.getenv("MC_SCHEM_PATH")
    if(not schemPath):
        raise SystemExit("No Schem Path specified")

    print(schemPath)
    schem.save(schemPath, schemOutput, mcschematic.Version.JE_1_20_1)
