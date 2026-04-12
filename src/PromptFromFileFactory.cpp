#include "PromptFromFileFactory.h"

#include "SystemPrompt.h"
#include "UserPrompt.h"
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>

std::string PromptFromFileFactory::loadContentFromFile(const std::string& filepath) const {
    std::ifstream file(filepath);

    // If the file doesn't exist then we throw (our toys out of the pram)
    if (!file) {
        throw std::runtime_error("File does not exist");
    }

    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

std::unique_ptr<SystemPrompt> PromptFromFileFactory::createSystemPromptFromFile(const std::string& filepath) const {
    return std::make_unique<SystemPrompt>(loadContentFromFile(filepath));
}

std::unique_ptr<UserPrompt> PromptFromFileFactory::createUserPromptFromFile(const std::string& filepath) const {
    return std::make_unique<UserPrompt>(loadContentFromFile(filepath));
}
