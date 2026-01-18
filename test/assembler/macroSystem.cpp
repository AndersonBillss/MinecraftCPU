#include "catch_amalgamated.hpp"
#include "../../src/assembler/macroSystem/MacroSystem.hpp"
#include "../../src/utils/runtimeError.hpp"
#include "ASTUtils.hpp"
#include <iostream>

TEST_CASE("MacroSystem stores variables across stacks")
{
    std::unique_ptr<AST::Node> node = std::make_unique<AST::Node>(AST::Node{
        begin : {0, 0},
        end : {0, 0},
        nodeType: AST::NodeType::PROGRAM,
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