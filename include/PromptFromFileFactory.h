#pragma once

#include "SystemPrompt.h"
#include "UserPrompt.h"
#include <string>
#include <memory>

// In actual OOP, this would probably be better as
// an AbstractFactory, oh well...
class PromptFromFileFactory {
private:
    std::string loadContentFromFile(const std::string& filepath) const;
public:
    std::unique_ptr<SystemPrompt> createSystemPromptFromFile(const std::string& filepath) const;
    std::unique_ptr<UserPrompt> createUserPromptFromFile(const std::string& filepath) const;
};
