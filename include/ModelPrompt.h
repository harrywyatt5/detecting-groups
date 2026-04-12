#pragma once

#include "Prompt.h"
#include <string>

class ModelPrompt : public Prompt {
private:
    std::string prompt;
public:
    ModelPrompt();
    const std::string& getPrompt() const override;
};
