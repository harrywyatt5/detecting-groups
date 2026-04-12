#pragma once

#include <string>

class Prompt {
public:
    enum class PromptType {
        SYSTEM,
        USER
    };

    virtual std::string getPrompt() const = 0;
    virtual ~Prompt() = default;
};
