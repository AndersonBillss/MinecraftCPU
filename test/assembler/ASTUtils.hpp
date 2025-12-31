#pragma once
#include "../../src/assembler/macroSystem/AST.hpp"
#include <string>

std::string stringifyTree(const AST::Node &tree, std::string tabs);

namespace AST
{
    inline std::ostream &operator<<(std::ostream &os,
                                    const std::unique_ptr<AST::Node> &tree)
    {
        return os << stringifyTree(*tree, "");
    }

    inline bool operator==(const std::unique_ptr<AST::Node> &a, const std::unique_ptr<AST::Node> &b)
    {
        if (
            !(
                a->begin == b->begin &&
                a->end == b->end &&
                a->identifier == b->identifier &&
                a->intValue == b->intValue &&
                a->nodeType == b->nodeType))
            return false;

        if (a->children.size() != b->children.size())
            return false;

        for (size_t i = 0; i < a->children.size(); i++)
        {
            auto &childA = a->children[i];
            auto &childB = b->children[i];
            bool childrenMatch = childA == childB;
            if (!childrenMatch)
                return false;
        }
        return true;
    }
}


std::unique_ptr<AST::Node> createNode(AST::NodeType type);
