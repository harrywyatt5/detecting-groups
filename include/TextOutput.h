#pragma once

#include <string>

class TextOutput {
private:
    std::string internal;
public:
    TextOutput(std::string internalStr) : internal(std::move(internalStr)) {}
};
