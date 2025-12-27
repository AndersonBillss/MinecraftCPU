#include "catch_amalgamated.hpp"
#include "../src/assembler/macroSystem/lexer.hpp"

inline bool operator==(const Lexer::Token &a,
                       const Lexer::Token &b)
{
    return a.begin == b.begin &&
           a.end == b.end &&
           a.type == b.type &&
           a.data == b.data;
}

inline std::ostream &operator<<(std::ostream &os,
                                const Lexer::Token &t)
{
    return os << "Token{begin=" << t.begin
              << ", end=" << t.end
              << ", type=" << t.type
              << ", data=\"" << t.data << "\"}";
}

std::vector<Lexer::Token> tokenizeHelper(std::string sourceCode)
{
    Lexer l;
    return l.tokenize(sourceCode);
}

TEST_CASE("Tokenize single line")
{
    std::string sourceCode = R"(
        LDI R1 1
    )";
    std::vector<Lexer::Token> expected{
        {{1, 8},
         {1, 10},
         Lexer::TokenType::VALUE,
         "LDI"},
        {{1, 12},
         {1, 13},
         Lexer::TokenType::VALUE,
         "R1"},
        {{1, 15},
         {1, 15},
         Lexer::TokenType::VALUE,
         "1"},
    };
    REQUIRE(tokenizeHelper(sourceCode) == expected);
}

TEST_CASE("Tokenize variable assignment")
{
    std::string sourceCode = R"(
        $test = 4
    )";
    std::vector<Lexer::Token> expected = {
        {{1, 8},
         {1, 12},
         Lexer::TokenType::SYMBOL,
         "$test"},
        {{1, 14},
         {1, 14},
         Lexer::TokenType::OPERATOR,
         "="},
        {{1, 16},
         {1, 16},
         Lexer::TokenType::VALUE,
         "4"},
    };
    REQUIRE(tokenizeHelper(sourceCode) == expected);
}

TEST_CASE("Tokenize multiple lines")
{
    std::string sourceCode = R"(
        LDI R1 1
        $test = 4
    )";
    std::vector<Lexer::Token> expected = {
        {{1, 8},
         {1, 10},
         Lexer::TokenType::VALUE,
         "LDI"},
        {{1, 12},
         {1, 13},
         Lexer::TokenType::VALUE,
         "R1"},
        {{1, 15},
         {1, 15},
         Lexer::TokenType::VALUE,
         "1"},
        {{1, 16},
         {1, 16},
         Lexer::TokenType::ENDLINE,
         "\n"},
        {{2, 8},
         {2, 12},
         Lexer::TokenType::SYMBOL,
         "$test"},
        {{2, 14},
         {2, 14},
         Lexer::TokenType::OPERATOR,
         "="},
        {{2, 16},
         {2, 16},
         Lexer::TokenType::VALUE,
         "4"}};
    REQUIRE(tokenizeHelper(sourceCode) == expected);
}

TEST_CASE("Tokenize functions")
{
    std::string sourceCode = R"(
        LDI R1 1
        $add = $1 $2 => $1 + $2
    )";
    std::vector<Lexer::Token> expected = {
        {{1, 8},
         {1, 10},
         Lexer::TokenType::VALUE,
         "LDI"},
        {{1, 12},
         {1, 13},
         Lexer::TokenType::VALUE,
         "R1"},
        {{1, 15},
         {1, 15},
         Lexer::TokenType::VALUE,
         "1"},
        {{1, 16},
         {1, 16},
         Lexer::TokenType::ENDLINE,
         "\n"},
        {{2, 8},
         {2, 11},
         Lexer::TokenType::SYMBOL,
         "$add"},
        {{2, 13},
         {2, 13},
         Lexer::TokenType::OPERATOR,
         "="},
        {{2, 15},
         {2, 16},
         Lexer::TokenType::SYMBOL,
         "$1"},
        {{2, 18},
         {2, 19},
         Lexer::TokenType::SYMBOL,
         "$2"},
        {{2, 21},
         {2, 22},
         Lexer::TokenType::OPERATOR,
         "=>"},
        {{2, 24},
         {2, 25},
         Lexer::TokenType::SYMBOL,
         "$1"},
        {{2, 27},
         {2, 27},
         Lexer::TokenType::OPERATOR,
         "+"},
        {{2, 29},
         {2, 30},
         Lexer::TokenType::SYMBOL,
         "$2"},
    };
    REQUIRE(tokenizeHelper(sourceCode) == expected);
}

TEST_CASE("Tokenize parentheses")
{
    std::string sourceCode = R"(
        (hello there)
    )";
    std::vector<Lexer::Token> expected = {
        {{1, 8},
         {1, 8},
         Lexer::TokenType::OPENINGPARENTHESE,
         "("},
        {{1, 9},
         {1, 13},
         Lexer::TokenType::VALUE,
         "hello"},
        {{1, 15},
         {1, 19},
         Lexer::TokenType::VALUE,
         "there"},
        {{1, 20},
         {1, 20},
         Lexer::TokenType::CLOSINGPARENTHESE,
         ")"},
    };
    REQUIRE(tokenizeHelper(sourceCode) == expected);
}
