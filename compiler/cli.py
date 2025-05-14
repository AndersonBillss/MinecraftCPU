from compile import compile
from createSchem import createSchem
from dotenv import load_dotenv
import os
load_dotenv()

import sys

if(len(sys.argv) < 2):
    print(
        "Welcome to the Minecraft CPU CLI\n"
        "To see help for this CLI, use the --help flag"
        )
    exit()

writeOutput = True
outFilePath = 'bin.mcexe'
outputMinecraft = True
outputMinecraftName = 'bin'
inFilePath = ""

i = 1
while i < len(sys.argv):
    arg = sys.argv[i]
    if(arg == "--no-bin"):
        writeOutput = False
    elif(arg == "--no-schem"):
        outputMinecraft = False
    elif(arg == "--bin-out"):
        if(i + 1 >= len(sys.argv)): raise SystemExit("You need to specify a file output name")
        outFilePath = sys.argv[i+1]
        i += 1 # Skip forward 1
    elif(arg == "--schem-out"):
        if(i + 1 >= len(sys.argv)): raise SystemExit("You need to specify a schematic name")
        outputMinecraftName = sys.argv[i+1]
        i += 1 # Skip forward 1
    elif(arg == "--help"):
        print(
            "Minecraft CPU CLI - Assemble .mcasm files into .mcexe binaries and Minecraft schematics\n"
            "\n"
            "Usage:\n"
            "  python compiler/cli.py <source_file.mcasm> [options]\n"
            "\n"
            "Options:\n"
            "  --no-bin                      Do not generate a .mcexe machine code file (default: enabled)\n"
            "  --no-schem                    Do not generate a Minecraft .schem file (default: enabled)\n"
            "  --bin-out <file>              Specify custom output filename for the .mcexe file\n"
            "                                (default: <source_file>.mcexe)\n"
            "  --schem-out <file>            Specify custom output filename for the .schem file\n"
            "                                (default: <source_file>.schem)\n"
            "\n"
            "Example:\n"
            "  python compiler/cli.py examples/fib.mcasm --bin-out demo.mcexe --no-schem\n"
        )
        exit()
    elif(arg[0:2] == "--"):
        raise SystemExit(f"Invalid argument: {arg}")
    else:
        if inFilePath: raise SystemExit(f"Invalid argument: {arg}")
        inFilePath = arg
    i += 1
    
if not inFilePath:
    raise SystemExit("No input path provided")

try:
    inFile = open(inFilePath, "r")
except IOError:
    raise SystemExit(f"Cannot find {inFilePath}")

programText = inFile.read()
programLines = compile(programText)

if(outputMinecraft):
    if not os.getenv("MC_SCHEM_PATH"): 
        raise SystemExit(
        "You cannot output minecraft schematic files unless you specify the MC_SCHEM_PATH environment variable\n" \
        "To compile a program without specifying a file, use the --no-schem flag"
        )
    createSchem(programLines, outputMinecraftName)

binFileText = ""
for line in programLines:
    binFileText += line + '\n'
binFileText = binFileText.strip()

if(writeOutput):
    try:
        outFile = open(outFilePath, "w")
    except IOError:
        raise SystemExit(f"can't open {outFilePath}")
    outFile.write(binFileText)
    outFile.close()

inFile.close()

if(outputMinecraft):
    print("To paste this code into Minecraft, use the following commands:")
    print(f"//schem load {outputMinecraftName}")
    print("//paste -a")