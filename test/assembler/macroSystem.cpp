#include "catch_amalgamated.hpp"
#include "../../src/assembler/macroSystem/MacroSystem.hpp"
#include "../../src/assembler/macroSystem/lexer.hpp"
#include "../../src/assembler/macroSystem/parser.hpp"
#include "../../src/utils/runtimeError.hpp"
#include "ASTUtils.hpp"
#include <iostream>

std::string evaluateMacroHelper(std::string &sourceCode)
{
    auto lexer = Lexer();
    auto tokens = lexer.tokenize(sourceCode);
    auto parser = Parser();
    auto ast = parser.parseTokens(tokens);

    std::string result = "";
    auto macroSystem = MacroSystem(ast.get());
    while (!macroSystem.done())
        result += macroSystem.getLine();

    return result;
}

TEST_CASE("MacroSystem stores variables across stacks")
{
    std::unique_ptr<AST::Node> node = std::make_unique<AST::Node>(AST::Node{
        begin : {0, 0},
        end : {0, 0},
        nodeType : AST::NodeType::PROGRAM,
        {},
        "",
        0
    });
    MacroSystem program(node.get());

    auto val1 = createNode(AST::NodeType::INT);
    val1->intValue = 43;
    program.setVariable("$test1", val1.get());
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.pushStack();
    auto val2 = createNode(AST::NodeType::INT);
    val2->intValue = 22;
    program.setVariable("$test2", val2.get());
    REQUIRE(program.getVariable("$test2")->intValue == 22);
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.pushStack();
    auto val3 = createNode(AST::NodeType::INT);
    val3->intValue = 87;
    program.setVariable("$test3", val3.get());
    REQUIRE(program.getVariable("$test3")->intValue == 87);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    REQUIRE(program.getVariable("$test2")->intValue == 22);
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    REQUIRE(program.getVariable("$test1")->intValue == 43);
}

TEST_CASE("MacroSystem evaluates simple expressions")
{
    std::string sourceCode = "1 + 2";
    std::string result = "3";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}