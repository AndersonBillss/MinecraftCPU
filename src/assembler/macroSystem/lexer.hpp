#pragma once
#include <set>
#include <vector>
#include <iostream>

std::set<char> operatorTokens {
    '+',
    '-',
    '*',
    '/',
    '=',
    '|',
    '&',
    '^',
    '<',
    '>',
    ',',
    '.',
};

std::vector<std::string> tokenize(std::string &block);