#include "catch_amalgamated.hpp"
#include "../src/assembler/assembler.hpp"
#include <string>

TEST_CASE("Assembler compiles basic assembly")
{
    std::string program = "LDI R0 1";
    std::string compiled = "00000001 00000000 00000001";
    REQUIRE(Assembler::compile(program) == compiled);
}

TEST_CASE("Assembler compiles assembly with symbols")
{
    std::string program;
    std::string compiled;

    program = R"(
        LDI     R1      5
        LDI     R2      1
        LDI     R3      1
        LDI     R4      255
        CMP     R1      R2
        BGT_IMM .next1
        JMP_IMM .fail

.next1  CMP     R2      R1
        BGT_IMM .fail
        BLT_IMM .next2
        JMP_IMM .fail
.next2  CMP     R1      R2
        BLT_IMM .fail
        SHOW    R2

.fail   SHOW    R4
)";
    compiled = R"(00000001 00000001 00000101
00000001 00000010 00000001
00000001 00000011 00000001
00000001 00000100 11111111
00011100 00000001 00000010
00000101 00000111 00000000
00000100 00001110 00000000
00011100 00000010 00000001
00000101 00001110 00000000
00000110 00001011 00000000
00000100 00001110 00000000
00011100 00000001 00000010
00000110 00001110 00000000
01000000 00000010 00000000
01000000 00000100 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);

    program = R"(
        LDI     R1  0
        LDI     R2  1
        LDI     R3  0
.fib    SHOW    R3
        LDI     R3  0
        ADD     R3  R1
        ADD     R3  R2
        COPY    R1  R2
        COPY    R2  R3
        JMP_IMM .fib

// Expected Output
// 0        1        2        3        5        8        13       21       34       55       89       144       233
// 00000000 00000001 00000010 00000011 00000101 00001000 00001101 00010101 00100010 00110111 01011001 010010000 11101001
)";
    compiled = R"(00000001 00000001 00000000
00000001 00000010 00000001
00000001 00000011 00000000
01000000 00000011 00000000
00000001 00000011 00000000
00010001 00000011 00000001
00010001 00000011 00000010
00010000 00000001 00000010
00010000 00000010 00000011
00000100 00000011 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);

    program = R"(
        LDI     R1      10
        LDI     R2      5
        LDI     R3      2
        SHOW    R3      // 2

        CALL    .fn3
        SHOW    R3      // 1

        CALL    .fn2
        SHOW    R2      // 4
        SHOW    R3      // 0

        CALL    .fn1
        SHOW    R1      // 6
        SHOW    R2      // 4
        SHOW    R3      // 255

.fn1    SUB     R1      R2
        CALL    .fn2    
        RET

.fn2    SUB     R2      R3
        CALL    .fn3
        RET

.fn3    SUB_IMM R3      1
        RET
        
// Expected Output
// 2        1        4        0        6        4        255
// 00000010 00000001 00000100 00000000 00000110 00000010 11111111
)";
    compiled = R"(00000001 00000001 00001010
00000001 00000010 00000101
00000001 00000011 00000010
01000000 00000011 00000000
00001001 00010011 00000000
01000000 00000011 00000000
00001001 00010000 00000000
01000000 00000010 00000000
01000000 00000011 00000000
00001001 00001101 00000000
01000000 00000001 00000000
01000000 00000010 00000000
01000000 00000011 00000000
00010010 00000001 00000010
00001001 00010000 00000000
00001010 00000000 00000000
00010010 00000010 00000011
00001001 00010011 00000000
00001010 00000000 00000000
00000011 00000011 00000001
00001010 00000000 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);

    program = R"(
        LDI     R1  5   //0
        SHOW    R1      //1 
        CALL    .sub    //2
        SHOW    R1      //3 
        CALL    .sub    //4
        SHOW    R1      //5
.sub    SUB_IMM R1  1   //6
        RET             //7
        LDI     R2  255 //8
        SHOW    R2      //9   

// Expected Output
// 5,        4,        3,
// 00000101  00000100  00000011 
)";
    compiled = R"(00000001 00000001 00000101
01000000 00000001 00000000
00001001 00000110 00000000
01000000 00000001 00000000
00001001 00000110 00000000
01000000 00000001 00000000
00000011 00000001 00000001
00001010 00000000 00000000
00000001 00000010 11111111
01000000 00000010 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);
}

TEST_CASE("Assembler compilers assembly with RAM instructions")
{
    std::string program;
    std::string compiled;

    program = R"(

    // Load 1, 2, 4, 8 into RAM
    LDI     R1  1
    STR     M0  R1
    LSHFT   R1 
    STR     M1  R1
    LSHFT   R1 
    STR     M2  R1
    LSHFT   R1 
    STR     M3  R1

    // Use a pointer to grab each memory location
    LDI     R1  0
    LOD_PTR R2  R1
    SHOW    R2
    ADD_IMM R1  1
    LOD_PTR R2  R1
    SHOW    R2
    ADD_IMM R1  1
    LOD_PTR R2  R1
    SHOW    R2
    ADD_IMM R1  1
    LOD_PTR R2  R1
    SHOW    R2

// Expected Output
// 1, 2, 4, 8

)";
    compiled = R"(00000001 00000001 00000001
00110000 00000001 00000000
00011011 00000001 00000000
00110000 00000001 00000001
00011011 00000001 00000000
00110000 00000001 00000010
00011011 00000001 00000000
00110000 00000001 00000011
00000001 00000001 00000000
00110011 00000010 00000001
01000000 00000010 00000000
00000010 00000001 00000001
00110011 00000010 00000001
01000000 00000010 00000000
00000010 00000001 00000001
00110011 00000010 00000001
01000000 00000010 00000000
00000010 00000001 00000001
00110011 00000010 00000001
01000000 00000010 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);

    program = R"(

LDI     R2      2

LDI     R1      1
STR     M1      R1
LDI     R1      12
STR     M2      R1
LDI     R1      33
STR     M3      R1
LDI     R1      99
STR     M4      R1
LDI     R1      67
STR     M5      R1
LDI     R1      83
STR     M6      R1

LOD     R2      M1
SHOW    R2      // 1
LOD     R2      M2
SHOW    R2      // 12
LOD     R2      M3
SHOW    R2      // 33
LOD     R2      M4
SHOW    R2      // 99
LOD     R2      M5
SHOW    R2      // 67
LOD     R2      M6
SHOW    R2      // 83

// Expected Output
// 1,        12,       33,       99,       67,       83
// 00000001  00001100  00100001  01100011  01000011  01010011

)";
    compiled = R"(00000001 00000010 00000010
00000001 00000001 00000001
00110000 00000001 00000001
00000001 00000001 00001100
00110000 00000001 00000010
00000001 00000001 00100001
00110000 00000001 00000011
00000001 00000001 01100011
00110000 00000001 00000100
00000001 00000001 01000011
00110000 00000001 00000101
00000001 00000001 01010011
00110000 00000001 00000110
00110001 00000010 00000001
01000000 00000010 00000000
00110001 00000010 00000010
01000000 00000010 00000000
00110001 00000010 00000011
01000000 00000010 00000000
00110001 00000010 00000100
01000000 00000010 00000000
00110001 00000010 00000101
01000000 00000010 00000000
00110001 00000010 00000110
01000000 00000010 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);

    program = R"(
            LDI         R1  0
            LDI         R2  1
            LDI         R3  10
            
.for        CMP         R1  R3
            BGT_IMM     .for_end
            CALL        .createList
            JMP_IMM     .for

.for_end    LOD         R4  M1 //2
            SHOW        R4
            LOD         R4  M2 //3
            SHOW        R4
            LOD         R4  M3 //4
            SHOW        R4
            LOD         R4  M4 //5
            SHOW        R4
            LOD         R4  M5 //6
            SHOW        R4
            LOD         R4  M6 //7
            SHOW        R4
            LOD         R4  M7 //8
            SHOW        R4
            LOD         R4  M8 //9
            SHOW        R4
            LOD         R4  M9 //10
            SHOW        R4
            LOD         R4  M10 //11
            SHOW        R4  
            LOD         R4  M11 //12
            SHOW        R4

.createList ADD_IMM     R1  1
            ADD_IMM     R2  1
            STR_PTR     R1  R2
            RET

// Expected Output
// 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12

)";
    compiled = R"(00000001 00000001 00000000
00000001 00000010 00000001
00000001 00000011 00001010
00011100 00000001 00000011
00000101 00000111 00000000
00001001 00011101 00000000
00000100 00000011 00000000
00110001 00000100 00000001
01000000 00000100 00000000
00110001 00000100 00000010
01000000 00000100 00000000
00110001 00000100 00000011
01000000 00000100 00000000
00110001 00000100 00000100
01000000 00000100 00000000
00110001 00000100 00000101
01000000 00000100 00000000
00110001 00000100 00000110
01000000 00000100 00000000
00110001 00000100 00000111
01000000 00000100 00000000
00110001 00000100 00001000
01000000 00000100 00000000
00110001 00000100 00001001
01000000 00000100 00000000
00110001 00000100 00001010
01000000 00000100 00000000
00110001 00000100 00001011
01000000 00000100 00000000
00000010 00000001 00000001
00000010 00000010 00000001
00110010 00000010 00000001
00001010 00000000 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);
};

TEST_CASE("Symbols can be placed on their own lines")
{
    std::string program = R"(
        LDI R1 1 
        LDI R1 1 
        LDI R1 1 
        LDI R1 1 
        LDI R1 1 
        LDI R1 1 

        .test<float>(int,int):<vector<float>>
        LDI R1 1
        LDI R1 1
        RET

        CALL .test<float>(int,int):<vector<float>>
        SHOW R1
    )";
    std::string compiled = R"(00000001 00000001 00000001
00000001 00000001 00000001
00000001 00000001 00000001
00000001 00000001 00000001
00000001 00000001 00000001
00000001 00000001 00000001
00000001 00000001 00000001
00000001 00000001 00000001
00001010 00000000 00000000
00001001 00000110 00000000
01000000 00000001 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);
}

TEST_CASE("Constants can be used")
{
    std::string program = R"(
        $ten = 10     
        $feetPerYard=3     

        LDI R1 $ten 
        LDI R2 $feetPerYard
        ADD R1 R2

        SHOW R1
    )";
    std::string compiled = R"(00000001 00000001 00001010
00000001 00000010 00000011
00010001 00000001 00000010
01000000 00000001 00000000)";
    REQUIRE(Assembler::compile(program) == compiled);
}

TEST_CASE("# comments can be used as well as // comments")
{
    std::string program = R"(
        LDI     R1      5
        LDI     R2      1
        LDI     R3      1   # This is a compiler-generate comment
        LDI     R4      255
        CMP     R1      R2
        BGT_IMM .next1
        JMP_IMM .fail

# This is a compiler-generated comment about what this symbol means
.next1  CMP     R2      R1
        BGT_IMM .fail
        BLT_IMM .next2
        JMP_IMM .fail
.next2  CMP     R1      R2  # This is another compiler-generated comment
        BLT_IMM .fail
        SHOW    R2

.fail   SHOW    R4  # This shows that the tests failed
)";

    std::string compiled = R"(00000001 00000001 00000101
00000001 00000010 00000001
00000001 00000011 00000001
00000001 00000100 11111111
00011100 00000001 00000010
00000101 00000111 00000000
00000100 00001110 00000000
00011100 00000010 00000001
00000101 00001110 00000000
00000110 00001011 00000000
00000100 00001110 00000000
00011100 00000001 00000010
00000110 00001110 00000000
01000000 00000010 00000000
01000000 00000100 00000000)";

    REQUIRE(Assembler::compile(program) == compiled);
}