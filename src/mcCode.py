from compile.compile import compile
from compile.createSchem import createSchem
from dotenv import load_dotenv
load_dotenv()

import sys

if(len(sys.argv) < 2):
    raise SystemExit("You need to specify a file to compile")
if(len(sys.argv) > 4):
    raise SystemExit("Only three operands allowed (input file, output option, and output path)")

inFilePath = sys.argv[1]
outFilePath = 'bin.mcexe'
writeOutput = False

if(len(sys.argv) > 2):
    outputFlag = sys.argv[2]
    if(outputFlag == "-o"):
        writeOutput = True
    else:
        raise SystemExit("Second operand must be -o (output option)")

if(len(sys.argv) > 3):
    outFilePath = sys.argv[3]


try:
    inFile = open(inFilePath, "r")
except IOError:
    raise SystemExit(f"can't open {inFilePath}")

programText = inFile.read()
programLines = compile(programText)

createSchem(programLines)

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

print("To paste this code, use the following commands:")
print("//schem load bin")
print("//paste -a")