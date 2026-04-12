#pragma once

#include <string>

class Prompt {
public:
    virtual const std::string& getPrompt() const = 0;
    virtual ~Prompt() = default;
};
