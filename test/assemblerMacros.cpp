#include "catch_amalgamated.hpp"
#include "../src/assembler/macroSystem/macroSystem.hpp"
#include "../src/assembler/macroSystem/oldLexer.hpp"
#include "../src/assembler/macroSystem/lexer.hpp"
#include "../src/utils/runtimeError.hpp"
#include <iostream>

TEST_CASE("Variables can be stored across stacks")
{
    MacroSystem program;
    program.setNumber("$test1", 43);
    int value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test2", 22);
    value = std::get<unsigned int>(program.getVariable("$test2"));
    REQUIRE(value == 22);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test3", 87);
    value = std::get<unsigned int>(program.getVariable("$test3"));
    REQUIRE(value == 87);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    value = std::get<unsigned int>(program.getVariable("$test2"));
    REQUIRE(value == 22);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);
}

TEST_CASE("Tokenization logic works correctly (old)")
{
    std::string sourceCode = R"(
        LDI R1 1
    )";
    std::vector<std::string> expected = {
        "LDI",
        "R1",
        "1"};
    REQUIRE(OldAsmMacroLexer::tokenize(sourceCode) == expected);

    sourceCode = R"(
        $test = 10
        LDI R1 1
        $test2 = (LDI R1 $test)
    )";
    expected = {
        "$test",
        "=",
        "10",
        "\n",
        "LDI",
        "R1",
        "1",
        "\n",
        "$test2",
        "=",
        "(LDI R1 $test)"};
    REQUIRE(OldAsmMacroLexer::tokenize(sourceCode) == expected);

    sourceCode = R"(
        $test=10 +
        LDI R1 1

        $test2 = (LDI R1 ($test * 2))
        NOP

        NOP

    )";
    expected = {
        "$test",
        "=",
        "10",
        "+",
        "\n",
        "LDI",
        "R1",
        "1",
        "\n",
        "$test2",
        "=",
        "(LDI R1 ($test * 2))",
        "\n",
        "NOP",
        "\n",
        "NOP"};
    REQUIRE(OldAsmMacroLexer::tokenize(sourceCode) == expected);

    sourceCode = R"(
        $test=10 +
        LDI R1 1

        $test2 = (LDI R1 ($test * 2))
        NOP

    .specialFunction:
        NOP

    )";
    expected = {
        "$test",
        "=",
        "10",
        "+",
        "\n",
        "LDI",
        "R1",
        "1",
        "\n",
        "$test2",
        "=",
        "(LDI R1 ($test * 2))",
        "\n",
        "NOP",
        "\n",
        ".specialFunction:",
        "\n",
        "NOP"};
    REQUIRE(OldAsmMacroLexer::tokenize(sourceCode) == expected);
}

inline bool operator==(const AsmMacroLexer::SourceLocation &a,
                       const AsmMacroLexer::SourceLocation &b)
{
    return a.line == b.line && a.column == b.column;
}
inline bool operator==(const AsmMacroLexer::Token &a,
                       const AsmMacroLexer::Token &b)
{
    return a.begin == b.begin &&
           a.end == b.end &&
           a.type == b.type &&
           a.data == b.data;
}
inline std::ostream &operator<<(std::ostream &os,
                                const AsmMacroLexer::SourceLocation &loc)
{
    return os << "(" << loc.line << "," << loc.column << ")";
}

inline std::ostream &operator<<(std::ostream &os,
                                const AsmMacroLexer::Token &t)
{
    return os << "Token{begin=" << t.begin
              << ", end=" << t.end
              << ", type=" << t.type
              << ", data=\"" << t.data << "\"}";
}
TEST_CASE("Lexer properly tokenizes source code")
{
    std::string sourceCode = R"(
        LDI R1 1
    )";
    std::vector<AsmMacroLexer::Token> expected{
        {{1, 8},
         {1, 10},
         AsmMacroLexer::TokenType::VALUE,
         "LDI"},
        {{1, 12},
         {1, 13},
         AsmMacroLexer::TokenType::VALUE,
         "R1"},
        {{1, 15},
         {1, 15},
         AsmMacroLexer::TokenType::VALUE,
         "1"},
    };
    AsmMacroLexer *l = new AsmMacroLexer();
    REQUIRE(l->tokenize(sourceCode) == expected);
    delete l;

    sourceCode = R"(
        $test = 4
    )";
    expected = {
        {{1, 8},
         {1, 12},
         AsmMacroLexer::TokenType::SYMBOL,
         "$test"},
        {{1, 14},
         {1, 14},
         AsmMacroLexer::TokenType::OPERATOR,
         "="},
        {{1, 16},
         {1, 16},
         AsmMacroLexer::TokenType::VALUE,
         "4"},
    };
    l = new AsmMacroLexer();
    REQUIRE(l->tokenize(sourceCode) == expected);
    delete l;

    sourceCode = R"(
        LDI R1 1
        $test = 4
    )";
    expected = {
        {{1, 8},
         {1, 10},
         AsmMacroLexer::TokenType::VALUE,
         "LDI"},
        {{1, 12},
         {1, 13},
         AsmMacroLexer::TokenType::VALUE,
         "R1"},
        {{1, 15},
         {1, 15},
         AsmMacroLexer::TokenType::VALUE,
         "1"},
        {{1, 16},
         {1, 16},
         AsmMacroLexer::TokenType::ENDLINE,
         "\n"},
        {{2, 8},
         {2, 12},
         AsmMacroLexer::TokenType::SYMBOL,
         "$test"},
        {{2, 14},
         {2, 14},
         AsmMacroLexer::TokenType::OPERATOR,
         "="},
        {{2, 16},
         {2, 16},
         AsmMacroLexer::TokenType::VALUE,
         "4"}};
    l = new AsmMacroLexer();
    REQUIRE(l->tokenize(sourceCode) == expected);
    delete l;

    sourceCode = R"(
        LDI R1 1
        $add = $1, $2 => $1 + $2
    )";
    expected = {
        {{1, 8},
         {1, 10},
         AsmMacroLexer::TokenType::VALUE,
         "LDI"},
        {{1, 12},
         {1, 13},
         AsmMacroLexer::TokenType::VALUE,
         "R1"},
        {{1, 15},
         {1, 15},
         AsmMacroLexer::TokenType::VALUE,
         "1"},
        {{1, 16},
         {1, 16},
         AsmMacroLexer::TokenType::ENDLINE,
         "\n"},
        {{2, 8},
         {2, 11},
         AsmMacroLexer::TokenType::SYMBOL,
         "$add"},
        {{2, 13},
         {2, 13},
         AsmMacroLexer::TokenType::OPERATOR,
         "="},
        {{2, 15},
         {2, 16},
         AsmMacroLexer::TokenType::SYMBOL,
         "$1"},
        {{2, 17},
         {2, 17},
         AsmMacroLexer::TokenType::OPERATOR,
         ","},
        {{2, 19},
         {2, 20},
         AsmMacroLexer::TokenType::SYMBOL,
         "$2"},
        {{2, 22},
         {2, 23},
         AsmMacroLexer::TokenType::OPERATOR,
         "=>"},
        {{2, 25},
         {2, 26},
         AsmMacroLexer::TokenType::SYMBOL,
         "$1"},
        {{2, 28},
         {2, 28},
         AsmMacroLexer::TokenType::OPERATOR,
         "+"},
        {{2, 30},
         {2, 31},
         AsmMacroLexer::TokenType::SYMBOL,
         "$2"},
    };
    l = new AsmMacroLexer();
    REQUIRE(l->tokenize(sourceCode) == expected);
    delete l;

    sourceCode = R"(
        (hello there)
    )";
    expected = {
        {{1, 8},
         {1, 8},
         AsmMacroLexer::TokenType::OPENINGPARENTHESE,
         "("},
        {{1, 9},
         {1, 13},
         AsmMacroLexer::TokenType::VALUE,
         "hello"},
        {{1, 15},
         {1, 19},
         AsmMacroLexer::TokenType::VALUE,
         "there"},
        {{1, 20},
         {1, 20},
         AsmMacroLexer::TokenType::CLOSINGPARENTHESE,
         ")"},
    };
    l = new AsmMacroLexer();
    REQUIRE(l->tokenize(sourceCode) == expected);
    delete l;
}

TEST_CASE("Evaluate variables and symbols")
{
    MacroSystem *m = new MacroSystem();
    std::string sourceCode = R"(
        $test = 10
        $test
    )";
    std::string expected = "10";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $test = hello there
        $test   
    )";
    expected = "hello there";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $test1 = hello
        $test2 = world 
        $test1 $test2
    )";
    expected = "hello world";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $test1 = hello
        $test2 = 1 
        $test1 $test2
    )";
    expected = "hello 1";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $test1 = hello
        $test2 = world 
        $test3 = $test1 $test2
        $test3 wow
    )";
    expected = "hello world wow";
    REQUIRE(m->evaluate(sourceCode) == expected);
}

TEST_CASE("Evaluate variables with expressions")
{
    MacroSystem *m = new MacroSystem();
    std::string sourceCode = R"(
        3 + 4
    )";
    std::string expected = "7";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
       $three = 3
       $three + 8 
    )";
    expected = "11";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
       $five = 5
       $eleven = 11
       $five + $eleven 
    )";
    expected = "16";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
       $thrirteen = 13
       $four = 4
       $seventeen = $four + $thrirteen 
       hello $seventeen
    )";
    expected = "hello 17";
    REQUIRE(m->evaluate(sourceCode) == expected);
}

TEST_CASE("Evaluate parentheses")
{
    MacroSystem *m = new MacroSystem();
    std::string sourceCode = R"(
        $test = (4 + 5)
        $test
    )";
    std::string expected = "9";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
    $test = (
        4 + 8
    )
    $test
    )";
    expected = "12";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
    (
        4 + 8
    )
    )";
    expected = "12";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
    (2 + 3) * 7
    )";
    expected = "35";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
    2 + (3 * 7)
    )";
    expected = "23";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
    $test = 2 + (3 * 7)
    $test2 = (($test + 1) * 4) / 5
    (HELLO $test2)
    )";
    expected = "HELLO 19";
    REQUIRE(m->evaluate(sourceCode) == expected);
}

TEST_CASE("Evaluate functions")
{
    MacroSystem *m = new MacroSystem();
    std::string sourceCode = R"(
        $testFn = $a => Hello $a
        $testFn 4
    )";
    std::string expected = "Hello 4";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $testFn = $a $b => $a + $b
        $test = $testFn 1 4
        $test
    )";
    expected = "5";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $testFn = $a $b => (
            $three = 3
            ($a + $b) * 3
        )
        $test = $testFn 1 4
        $test
    )";
    expected = "15";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $testFn = $a $b $c => (
            ($a + $b) * $c
        )
        $mul = 8
        $test = $testFn 3 7 $mul
        $test
    )";
    expected = "80";
    REQUIRE(m->evaluate(sourceCode) == expected);

    delete m;
    m = new MacroSystem();
    sourceCode = R"(
        $a = 3
        $testFn = $a $b $c => (
            ($a + $b) * $c
        )
        $mul = 8
        $test = $testFn 3 7 $mul
        $test
    )";
    expected = "80";
    REQUIRE(m->evaluate(sourceCode) == expected);
}

TEST_CASE("replace location symbols")
{
    MacroSystem *m = new MacroSystem();
    std::string sourceCode = R"(
        .testSymbol
        hello .testSymbol
    )";
    std::string expected = "hello 0";
    REQUIRE(m->replaceLocationSymbols(m->evaluate(sourceCode)) == expected);

    m = new MacroSystem();
    sourceCode = R"(
        hello
        hello
        hello
        hello

    .testSymbol
        hello
        hello
        hello .testSymbol
        hello
    )";
    expected = R"(hello
hello
hello
hello

hello
hello
hello 4
hello)";
    REQUIRE(m->replaceLocationSymbols(m->evaluate(sourceCode)) == expected);

    m = new MacroSystem();
    sourceCode = R"(
            hello
            hello
            hello
            hello
.testSymbol hello
            hello
            hello .testSymbol
            hello
    )";
    expected = R"(hello
hello
hello
hello
hello
hello
hello 4
hello)";
    REQUIRE(m->replaceLocationSymbols(m->evaluate(sourceCode)) == expected);
}