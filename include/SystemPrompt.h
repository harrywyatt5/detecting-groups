#pragma once

#include "Prompt.h"
#include <string>

class SystemPrompt : public Prompt {
protected:
    std::string prompt;

    static const std::string PREFIX;
    static const std::string SUFFIX;
public:
    SystemPrompt(const std::string& basePrompt);
    const std::string& getPrompt() const override;
};
