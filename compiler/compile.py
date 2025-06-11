import os

class operation:
    bincode = ''
    operation = ''
    operands = []
    def __init__(self, bincode, operands):
        self.bincode = bincode
        self.operation = operands[0]
        self.operands = operands[1:]

def removeComments(string):
    lines = string.split('\n')
    removedCommentLines = []
    for line in lines:
        commentIndex = line.find('//')
        if commentIndex != -1:
            line = line[:commentIndex]
        if line.strip() != '':
            removedCommentLines += [line]
    return '\n'.join(removedCommentLines)

def intToBin(number):
    number = int(number)
    if number < 0:
        return bin(number + 256)[2:]

    return bin(number)[2:]

def make8Bits(number):
    number = str(number)
    while len(number) < 8:
        number = "0" + number
    return number

def processLine(line, instructionRules):
    if line.strip() == "":
        return("")
    splitLines = line.split()
    opcode = splitLines[0]

    operands = splitLines[1:]


    operationInstruction = None
    for item in instructionRules:
        if item.operation == opcode:
            operationInstruction = item

    if operationInstruction == None:
        raise SyntaxError(f"operation {opcode} does not exist")
    
    binaryEncodedOperations = [f"{operationInstruction.bincode}"]

    if len(operands) != len(operationInstruction.operands):
        raise SyntaxError(f"operation {opcode} has {len(operationInstruction.operands)} operands")
    
    for (i, operand) in enumerate(operands):
        operandType = operationInstruction.operands[i]
        if operand[0] == '.':
            operand = symbolMap[operand]
        if operandType == "REG":
            if operand[0] != "R": raise SyntaxError(f"Operand {i+1} of {opcode} should start with 'R'")
            regNum = int(operand[1:])
            if regNum > 31: raise SyntaxError(f"Register addresses can't be greater than 31")
            binaryEncodedOperations.append(f"{make8Bits(intToBin(regNum))}")
        elif operandType == "RAM":
            memNum = int(operand[1:])
            if operand[0] != "M": raise SyntaxError(f"Operand {i+1} of {opcode} should start with 'R'")
            if memNum > 255: raise SyntaxError(f"RAM addresses can't be greater than 255")
            binaryEncodedOperations.append(f"{make8Bits(intToBin(memNum))}")
        elif operandType == "NUM":
            binaryEncodedOperations.append(f"{make8Bits(intToBin(operand))}")
    while len(binaryEncodedOperations) < 3:
        binaryEncodedOperations.append("00000000")

    if opcode == "STR" or opcode == "STR_PTR": # The operands for these operations are swapped to work with hardware
        tmp = binaryEncodedOperations[1]
        binaryEncodedOperations[1] = binaryEncodedOperations[2]
        binaryEncodedOperations[2] = tmp

    binaryLine = " ".join(binaryEncodedOperations).strip()
        
    return(f"{binaryLine}")

symbolMap = {}
def processSymbols(programText):
    programLines = programText.split('\n')
    removedSymbolLines = []
    for (i, line) in enumerate(programLines):
        splitLine = line.split()
        if splitLine[0][0] == ".":
            symbolMap[splitLine[0]] = i
            splitLine[0] = ''
        removedSymbolLines.append(' '.join(splitLine).strip())
    
    return '\n'.join(removedSymbolLines).strip() 

def compile(programText):
    instructionRules = []
    instructionTableText = ""

    script_dir = os.path.dirname(os.path.abspath(__file__))
    instruction_path = os.path.join(script_dir, "instruction_table.txt")
    with open(instruction_path, "r") as instructionFile:
        instructionTableText = instructionFile.read()
    instructionTableText = removeComments(instructionTableText)
    instructionLines = instructionTableText.split("\n")
    sanitizedLines = []
    for line in instructionLines:
        line = line.strip()
        if line != '':
            sanitizedLines += [line]

    for line in sanitizedLines:
        splitLine = line.split(' - ')
        if len(splitLine) > 2:
            binCode = splitLine[1].strip()
            operands = splitLine[2].split()
            operands = list(map(lambda item: item.strip(), operands))
            instructionRules += [operation(binCode, operands)]

    programText = removeComments(programText)
    programText = processSymbols(programText)
    programLines = programText.split('\n')

    binLines = []
    for line in programLines:
        if line.strip() != "":
            binLines += [processLine(line, instructionRules)]
    return binLines


