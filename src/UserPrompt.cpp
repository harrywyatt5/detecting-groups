#include "UserPrompt.h"

#include <string>

const std::string UserPrompt::PREFIX = "<|turn>user\n";
const std::string UserPrompt::SUFFIX = "<turn|>\n";

UserPrompt::UserPrompt(const std::string& basePrompt) {
    prompt = UserPrompt::PREFIX + basePrompt + UserPrompt::SUFFIX;
}

const std::string& UserPrompt::getPrompt() const {
    return prompt;
}
