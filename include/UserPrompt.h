#pragma once

#include "Prompt.h"
#include <string>

class UserPrompt : public Prompt {
protected:
    std::string prompt;
    UserPrompt() {}

    static const std::string PREFIX;
    static const std::string SUFFIX;
public:
    UserPrompt(const std::string& basePrompt);
    const std::string& getPrompt() const override;
};
