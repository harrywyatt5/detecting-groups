#include "SystemPrompt.h"

#include <string>

const std::string SystemPrompt::PREFIX = "<bos><|turn>system\n";
const std::string SystemPrompt::SUFFIX = "\n<turn|>\n";

SystemPrompt::SystemPrompt(const std::string& basePrompt) {
    prompt = SystemPrompt::PREFIX + basePrompt + SystemPrompt::SUFFIX;
}

const std::string& SystemPrompt::getPrompt() const {
    return prompt;
}
